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

std::istream& operator >> (std::istream& in, Point& temp) {
    in >> temp.x >> temp.y;
    return in;
}

struct ConvexHull {
    ConvexHull(const vector<Point>& pointers) :
        points_(pointers), perimeter_(0) {}

    void algGraham();

    long double findPerimeter();
private:
    vector<Point> points_;//����� �� ���������
    Point p0_;//����� ������ � ����� �����. �� �� ����� ���� ������
    long double perimeter_;
    stack<Point> S_;
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
        long long det = crossProduct(p0_, p1, p2);
        if (det == 0) {//��������������
            if (dist(p0_, p2) > dist(p0_, p1)) {
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
        p0_(p0) {}

private:
    Point p0_;
};

int main() {
    long long n;
    std::cin >> n;
    vector<Point> points;

    for (int i = 0; i < n; ++i) {
        Point temp;
        std::cin >> temp;
        
        points.push_back(temp);
    }

    ConvexHull ans(points);

    ans.algGraham();
    std::cout << std::setprecision(10) << ans.findPerimeter();

    return 0;
}

void ConvexHull::algGraham() {
    long long n = static_cast<long long>(points_.size());
    // ���� ������ �����
    long long ymin = points_[0].y;
    long long min = 0;
    for (long long i = 1; i < n; i++) {
        long long y = points_[i].y;

        if ((y < ymin) || (ymin == y && points_[i].x < points_[min].x)) {// ���� �� y, ����� �� x. ����� ������ => ����� ����� �� ������ 
            ymin = points_[i].y;
            min = i;
        }
    }

    std::swap(points_[0], points_[min]);//�� ������� ������� ������ ��� �������

    // �������� ������������ ���� ����� �� ����. �� ����� ����� �������� �������� ����, ����� �� ���� � ���� �������� :(
    Compare comp(points_[0]);
    vector<Point>::iterator it = points_.begin();
    ++it;
    std::sort(it, points_.end(), comp);

    //���������� � ���� ������ ��� �����. ����� 0 � 1 ����� ����������� ���. ��������
    S_.push(points_[0]);
    S_.push(points_[1]);
    S_.push(points_[2]);

    for (int i = 3; i < n; ++i) {
        Point np = points_[i];

        while (S_.size() > 1 && crossProduct(nextToTop(S_), S_.top(), points_[i]) != 2) {// �� �������, �������
            S_.pop();
        }
        S_.push(points_[i]);//������ ������� ����� � ����
    }
}

long double ConvexHull::findPerimeter() {
    Point p0 = S_.top();
    Point p1 = S_.top();
    S_.pop();

    while (!S_.empty()) {
        Point p2 = S_.top();
        perimeter_ += sqrt(dist(p1, p2));
        p1 = p2;
        S_.pop();
    }
    perimeter_ += sqrt(dist(p0, p1));

    return perimeter_;
}
