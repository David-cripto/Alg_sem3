#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>

using std::string;
using std::vector;

const int alphabet = 26;

struct Vertex {
	int left;
	int right;
	int suf;
	int parent;
	bool has_dollar;
	bool has_la;
	std::map<char, int> edges;

	Vertex(int left = -1, int right = -1, int parent = -1) :
		left(left), right(right), parent(parent), suf(0),
		has_dollar(false), has_la(false)
	{}
};

struct Suftree {
	Suftree(string& text);

	string findAnsString(long long k, int ind_of_dol, int ind_of_lat);

private:
	void DFSForMarkVertexes(int ver, int ind_of_dol, int ind_of_lat);
	void DFSForFindVertexes(int ver, int& ver_ans, long long& k);
	void algorithmOfUkkonen_(char ch);

	int cur_v_;
	int cur_pos_;
	int cur_pos_str_;
	string text_;
	vector<Vertex> tree_;
};


Suftree::Suftree(string& text) :
	cur_v_(0), cur_pos_(0), cur_pos_str_(0),
	text_(text), tree_(2, Vertex())
{
	const char first_char = 'a';
	int len = static_cast<int>(text.length());

	for (int i = 0; i < alphabet; ++i) { // 1 -- ������ �������. �������� ��� ������� �������.
		char temp = first_char + i;
		tree_[1].edges[temp] = 0;
	}

	tree_[1].edges['#'] = 0;
	tree_[1].edges['$'] = 0;
	tree_[0].suf = 1;

	for (int i = 0; i < len; ++i) {
		char ch = text[i];
		algorithmOfUkkonen_(ch);
		++cur_pos_str_;
	}
}

string Suftree::findAnsString(long long k, int ind_of_dol, int ind_of_lat) {
	int ver_ans = 0;
	string ans_str;

	DFSForMarkVertexes(0, ind_of_dol, ind_of_lat);

	DFSForFindVertexes(0, ver_ans, k);

	if (k > 0) { // �� ����� ������� 
		return ans_str;
	} else {
		while (ver_ans != 0) {//��� � ������
			int iter = 0;
			int len = tree_[ver_ans].right - tree_[ver_ans].left + k;
			while (iter <= len) {
				ans_str.push_back(text_[len - iter + tree_[ver_ans].left]);
				++iter;
			}
			k = 0;
			ver_ans = tree_[ver_ans].parent;
		}
	}

	return ans_str;
}

void Suftree::DFSForMarkVertexes(int ver, int ind_of_dol, int ind_of_lat) {
	if (tree_[ver].right >= ind_of_dol && tree_[ver].left <= ind_of_dol) {
		tree_[ver].has_dollar = true;//�������� $
	}

	if (tree_[ver].left > ind_of_dol && tree_[ver].right == ind_of_lat) {//�������� #
		tree_[ver].has_la = true;
	}

	for (auto it = tree_[ver].edges.begin(); it != tree_[ver].edges.end(); ++it) {
		DFSForMarkVertexes(it->second, ind_of_dol, ind_of_lat);//DFS �� ���������

		if (!tree_[ver].has_dollar && tree_[it->second].has_dollar) {//�������� $ ����� �������
			tree_[ver].has_dollar = true;
		}

		if (!tree_[ver].has_la && tree_[it->second].has_la) {//�������� # ����� �������
			tree_[ver].has_la = true;
		}
	}
}

void Suftree::DFSForFindVertexes(int ver, int& ver_ans, long long& k) {
	if (ver != 0) {
		k -= (tree_[ver].right - tree_[ver].left + 1);
	}

	if (k <= 0) {
		ver_ans = ver;
		return;
	}

	for (auto it = tree_[ver].edges.begin(); it != tree_[ver].edges.end(); ++it) {
		if (!tree_[it->second].has_dollar || !tree_[it->second].has_la) {
			continue;
		}

		DFSForFindVertexes(it->second, ver_ans, k);//DFS �� ���������

		if (k <= 0) {
			return;
		}
	}
}

void Suftree::algorithmOfUkkonen_(char ch) {
	int len = static_cast<int>(text_.length());

	if (tree_[cur_v_].right < cur_pos_) { // ����� �� �����, ����� ��������� � ���������� �������
		if (tree_[cur_v_].edges.find(ch) == tree_[cur_v_].edges.end()) { // ��� ������ �� ������� �������, ������ ����. ����� 2�
			tree_.push_back(Vertex(cur_pos_str_, len - 1, cur_v_));
			tree_[cur_v_].edges[ch] = static_cast<int>(tree_.size()) - 1;
			cur_v_ = tree_[cur_v_].suf;
			cur_pos_ = tree_[cur_v_].right + 1;
			algorithmOfUkkonen_(ch);
			return;
		}

		cur_v_ = tree_[cur_v_].edges[ch];
		cur_pos_ = tree_[cur_v_].left;
	}

	if (cur_pos_ == -1 || ch == text_[cur_pos_]) {
		++cur_pos_; // ��������� ����� 3. ������ ��� ������
		return;
	}
	// ����� 2� ��������� �����. ��, ��� �� ���� ��� ������ :)
	tree_.push_back(Vertex(tree_[cur_v_].left, cur_pos_ - 1, tree_[cur_v_].parent));
	int n_ver = static_cast<int>(tree_.size()) - 1;
	tree_[n_ver].edges[text_[cur_pos_]] = cur_v_;
	tree_[cur_v_].left = cur_pos_;
	tree_[tree_[cur_v_].parent].edges[text_[tree_[n_ver].left]] = n_ver;
	tree_[cur_v_].parent = n_ver;

	tree_.push_back(Vertex(cur_pos_str_, len - 1, n_ver));
	int n_ver_2 = static_cast<int>(tree_.size()) - 1;
	tree_[n_ver].edges[text_[cur_pos_str_]] = n_ver_2;

	cur_v_ = tree_[tree_[n_ver].parent].suf;
	cur_pos_ = tree_[n_ver].left;

	while (cur_pos_ <= tree_[n_ver].right) { // ������� ����� ����. 
		cur_v_ = tree_[cur_v_].edges[text_[cur_pos_]];
		cur_pos_ += 1 + tree_[cur_v_].right - tree_[cur_v_].left;
	}

	if (cur_pos_ == tree_[n_ver].right + 1) {// �������
		tree_[n_ver].suf = cur_v_;
	}
	else {// �� �������
		tree_[n_ver].suf = n_ver_2 + 1;
	}

	cur_pos_ = tree_[cur_v_].right - (cur_pos_ - tree_[n_ver].right) + 2;// ������� ������� �� ����� �����

	algorithmOfUkkonen_(ch);
}

int main() {
	std::ios::sync_with_stdio(false);
	string s, t;
	long long k;
	std::cin >> s >> t >> k;
	string temp = s + "$" + t + "#";
	int ind_of_dol = static_cast<int>(s.length());
	int ind_of_lat = static_cast<int>(s.length()) + static_cast<int>(t.length()) + 1;

	Suftree tree(temp);

	string ans_str = tree.findAnsString(k, ind_of_dol, ind_of_lat);

	int len_str = static_cast<int>(ans_str.length());

	if (len_str == 0) {
		std::cout << -1;
		return 0;
	}

	for (int i = 0; i < len_str; ++i) {
		std::cout << ans_str[len_str - 1 - i];
	}

	return 0;
}