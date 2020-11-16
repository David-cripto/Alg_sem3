#include <iostream>
#include <stack>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iomanip>

using std::stack;
using std::vector;
using std::iterator;
using std::pair;

const double eps = 1e-9;// ��� ���������� � ���������

struct Segment;
bool intersect(const Segment& a, const Segment& b);

struct Point {
    Point(long long x = 0, long long y = 0) :
        x(x), y(y) {}

    long long x;
    long long y;
};

struct Segment {
    Point begin;
    Point end;
    //����� ������� ��� ������ ���������������� ������� ������
    int number;

    Segment(const Point& begin, const Point& end, int number) :
        begin(begin), end(end), number(number) {}

    friend bool operator< (const Segment& a, const Segment& b);
    double get_y(double x) const {
        //������� � ���� �� ������� ��������(���� �������), �� ������ ������ �
        if (std::abs(begin.x - end.x) < eps) {
            return begin.y;
        }

        return begin.y + (end.y - begin.y) * (x - begin.x) / (end.x - begin.x);
    }
};

struct Event {
    // ������� ���������� �� �
    int event_x;

    //����� �������
    int numner_of_segment;

    //��������� ��� ������� �������
    bool is_begin;

    Event(int event_x, int number_of_segment, bool is_begin) :
        event_x(event_x), numner_of_segment(number_of_segment),
        is_begin(is_begin) {}
};

//���������� ��� Event
struct CompEvents {
    bool operator()(const Event& e1, const Event& e2) {
        if (std::abs(e1.event_x - e2.event_x) < eps) {// ������� �� ����� ���������
            return e1.is_begin > e2.is_begin;
        }

        return e1.event_x < e2.event_x;
    }
}cmp_events;

//����:
//������� ������������ ������ �� ����� �������������,
//������� ����� ��������� ����� � 
//(�� ����� ���� ����� ������� ������������ �����������), 
//� �������� event -- ������ ���������� � �������� �������
//��� ���������� ��������� ������� � ��������,
//��� �������� ��������� �������
void solve(const vector<Segment>& a) {
    int n = static_cast<int>(a.size());
    pair<int, int> ans = std::make_pair(-1, -1);

    vector<Event> events;

    //��������� ����� �������� �� � -- ���� �������
    for (int i = 0; i < n; ++i) {
        events.push_back(Event(std::min(a[i].begin.x, a[i].end.x), a[i].number, true));
        events.push_back(Event(std::max(a[i].begin.x, a[i].end.x), a[i].number, false));
    }

    std::sort(events.begin(), events.end(), cmp_events);
    int n_e = static_cast<int>(events.size());

    //������ ������� �� �������� ������������� �� ������ ������ � sweep line
    std::set<Segment> order_of_segments;

    //��������� �� ������� ��� �������� ������� � �������
    vector<std::set<Segment>::iterator> number_of_seg;
    number_of_seg.resize(n);

    //����������� �� ���� events, � ���� ���������, ���� ������� � ����������
    for (int i = 0; i < n_e; ++i) {
        //���������� �������
        if (events[i].is_begin) {
            std::set<Segment>::iterator next;
            std::set<Segment>::iterator temp;
            std::set<Segment>::iterator prev;

            next = order_of_segments.lower_bound(a[events[i].numner_of_segment]);
            temp = next;
            if (temp == order_of_segments.begin()) {
                prev = order_of_segments.end();
            } else {
                prev = --temp;
            }

            //����� �����������
            if (next != order_of_segments.end() && intersect(*next, a[events[i].numner_of_segment])) {
                ans = std::make_pair(next->number, events[i].numner_of_segment);
                break;
            }
            if (prev != order_of_segments.end() && intersect(*prev, a[events[i].numner_of_segment])) {
                ans = std::make_pair(prev->number, events[i].numner_of_segment);
                break;
            }

            number_of_seg[events[i].numner_of_segment] = order_of_segments.insert(next, a[events[i].numner_of_segment]);
        } else {//��������
            std::set<Segment>::iterator next;
            std::set<Segment>::iterator temp;
            std::set<Segment>::iterator prev;

            temp = number_of_seg[events[i].numner_of_segment];
            next = ++temp;
            temp = number_of_seg[events[i].numner_of_segment];
            if (temp == order_of_segments.begin()) {
                prev = order_of_segments.end();
            }
            else {
                prev = --temp;
            }

            if (next != order_of_segments.end() && prev != order_of_segments.end() && intersect(*next, *prev)) {
                ans = std::make_pair(prev->number, next->number);
                break;
            }

            order_of_segments.erase(number_of_seg[events[i].numner_of_segment]);
        }
    }

    if (ans.first == -1 && ans.second == -1) {
        std::cout << "NO" << "\n";
    } else {
        std::cout << "YES" << "\n" << ans.first + 1 << " " << ans.second + 1;
    }
}

//������� �� �������� �� � � �
bool intersectOfProjection(double l1, double r1, double l2, double r2) {
    if (l1 > r1) {
        std::swap(l1, r1);
    }
    if (l2 > r2) {
        std::swap(l2, r2);
    }
    return std::max(l1, l2) <= std::min(r1, r2);
}

//���������� ���� ������������
int crossProduct(const Point& a, const Point& b, const Point& c) {
    double temp = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return std::abs(temp) < eps ? 0 : temp > 0 ? 1 : -1;
}

bool intersect(const Segment& a, const Segment& b) {
    //������� ��������� �� ���������� �� �
    bool first_step = intersectOfProjection(a.begin.x, a.end.x, b.begin.x, b.end.x);

    //����� �� �
    bool sec_step = intersectOfProjection(a.begin.y, a.end.y, b.begin.y, b.end.y);

    //������ ��������� ���� ������� � � ������� ������� ���� ��� ������������
    bool th_step = crossProduct(a.begin, a.end, b.begin) * crossProduct(a.begin, a.end, b.end) <= 0;

    //��������� � ������ ��������
    bool four_step = crossProduct(b.begin, b.end, a.begin) * crossProduct(b.begin, b.end, a.end) <= 0;

    return first_step && sec_step && th_step && four_step;
}

bool operator< (const Segment& a, const Segment& b) {
    double x = std::max(std::min(a.begin.x, a.end.x), std::min(b.begin.x, b.end.x));
    return a.get_y(x) < b.get_y(x);
}


int main() {
    //while (true)
    //{
    //    int n;
    //    n = rand() % 3 + 1;

    //    vector<Segment> a;

    //    std::cout << "/////////////////////" << "\n";

    //    std::cout << n << "\n";

    //    for (int i = 0; i < n; ++i) {
    //        Point begin;
    //        Point end;

    //        /*std::cin >> begin.x >> begin.y >> end.x >> end.y;*/

    //        begin.x = rand() % 5;
    //        begin.y = rand() % 5;
    //        end.x = rand() % 5;
    //        end.y = rand() % 5;

    //        std::cout << begin.x << " " << begin.y << " " << end.x << " " << end.y << "\n";

    //        Segment temp(begin, end, i);
    //        a.push_back(temp);
    //    }

    //    solve(a);
    //}

    int n;
    std::cin >> n;

    vector<Segment> a;

    for (int i = 0; i < n; ++i) {
        Point begin;
        Point end;

        std::cin >> begin.x >> begin.y >> end.x >> end.y;

        Segment temp(begin, end, i);
        a.push_back(temp);
    }

    solve(a);

    return 0;
}