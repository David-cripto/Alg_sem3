#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iomanip>

using std::stack;
using std::vector;
using std::iterator;

struct Point {
    Point(long long x = 0, long long y = 0) :
        x(x), y(y) {}

    long long x;
    long long y;
};

struct ConvexHull {
    ConvexHull(vector<Point>& pointers) :
        pointers(pointers), perimetr(0) {}

    void algGraham();

private:
    vector<Point> pointers;//����� �� ���������
    Point p0;//����� ������ � ����� �����. �� �� ����� ���� ������
    long double perimetr;
};

Point nextToTop(stack<Point>& S) {
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

//������� ���������� ����� �������
long double dist(Point p1, Point p2) {
    return static_cast<long double>((p1.x - p2.x))* static_cast<long double>((p1.x - p2.x)) + static_cast<long double>((p1.y - p2.y))* static_cast<long double>((p1.y - p2.y));
}

//������� ������ pp1 � p1p2. ������� �� ���������� � ������� ���������� ������������
int crossProduct(Point p, Point p1, Point p2) {
    long long det = (p1.x - p.x) * (p2.y - p1.y) - (p1.y - p.y) * (p2.x - p1.x);//������������ �������

    if (det == 0) {//������������ ������� ����� ���� => ������� �����������
        return 0;
    }
    else if (det > 0) {//������ �������
        return 2;
    }
    else {//�� �������
        return 1;
    }
}

// ���������� ��� qsort
struct Compare {
    bool operator()(const Point& p1, const Point& p2) {
        //������� ����������
        long long det = crossProduct(p0, p1, p2);
        if (det == 0) {//��������������
            if (dist(p0, p2) > dist(p0, p1)) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (det == 2) {//������ �������
            return true;
        }
        else {//�� �������
            return false;
        }
    }
    Compare(Point p0) :
        p0(p0) {}

private:
    Point p0;
};

int main() {
    long long n;
    std::cin >> n;
    vector<Point> pointers;

    for (int i = 0; i < n; ++i) {
        long long x;
        long long y;
        std::cin >> x >> y;
        Point temp(x, y);
        pointers.push_back(temp);
    }

    ConvexHull ans(pointers);
    ans.algGraham();
    return 0;
}

void ConvexHull::algGraham() {
    long long n = static_cast<long long>(pointers.size());
    // ���� ������ �����
    long long ymin = pointers[0].y;
    long long min = 0;
    for (long long i = 1; i < n; i++) {
        long long y = pointers[i].y;

        if ((y < ymin) || (ymin == y && pointers[i].x < pointers[min].x)) {// ���� �� y, ����� �� x. ����� ������ => ����� ����� �� ������ 
            ymin = pointers[i].y;
            min = i;
        }
    }

    std::swap(pointers[0], pointers[min]);//�� ������� ������� ������ ��� �������

    // �������� ������������ ���� ����� �� ����. �� ����� ����� �������� �������� ����, ����� �� ���� � ���� �������� :(
    Compare comp(pointers[0]);
    vector<Point>::iterator it = pointers.begin();
    ++it;
    std::sort(it, pointers.end(), comp);

    //���������� � ���� ������ ��� �����. ����� 0 � 1 ����� ����������� ���. ��������
    stack<Point> S;
    S.push(pointers[0]);
    S.push(pointers[1]);
    S.push(pointers[2]);

    for (int i = 3; i < n; ++i) {
        Point np = pointers[i];

        //while (crossProduct(nextToTop(S), S.top(), pointers[i]) == 0) {//����� �� ����� ������
        //    

        //}

        while (S.size() > 1 && crossProduct(nextToTop(S), S.top(), pointers[i]) != 2) {// �� �������, �������
            S.pop();
        }
        S.push(pointers[i]);//������ ������� ����� � ����
    }

    Point p0 = S.top();
    Point p1 = S.top();
    S.pop();

    while (!S.empty()) {
        Point p2 = S.top();
        perimetr += sqrt(dist(p1, p2));
        p1 = p2;
        S.pop();
    }
    perimetr += sqrt(dist(p0, p1));

    std::cout << std::setprecision(10) << perimetr;
}
