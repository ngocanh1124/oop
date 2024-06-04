#include <iostream>
#include <vector>
#include <string>
#include <utility> 
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <random>
#include<algorithm>

using namespace std;
vector<int> GetRandom(int n, int S) {
    if (n <= 0 || S < n) {
        throw invalid_argument("Invalid");
    }
    vector<double> random_values(n);
    random_device rd;
    default_random_engine gen(rd()); // Khởi tạo một generator (gen) với thuật toán mặc định và sử dụng random_device (rd) để tạo seed.
    normal_distribution<> d(0, 1000); // Phân phối chuẩn (normal distribution) với trung bình (mean) là 0 và độ lệch chuẩn (standard deviation) là 1000.
    for (int i = 0; i < n; ++i) {
        random_values[i] = d(gen); // Sinh n giá trị ngẫu nhiên theo phân phối chuẩn đã khai báo và lưu vào vector random_values.
    }
    double min_val = *min_element(random_values.begin(), random_values.end()); // Tìm giá trị nhỏ nhất trong vector random_values.
    for (double& value : random_values) { // Trừ đi giá trị nhỏ nhất đã tìm được khỏi mỗi phần tử của vector random_values.
        value -= min_val;
    }
    double sum = accumulate(random_values.begin(), random_values.end(), 0.0); // Tính tổng của tất cả các phần tử trong vector random_values.
    for (double& value : random_values) {
        value = (value / sum) * S; // Chia mỗi giá trị trong vector random_values cho tổng sum, sau đó nhân với S để đảm bảo tổng của chúng là S.
    }
    vector<int> result(n);
    int int_sum = 0;
    for (int i = 0; i < n; ++i) { // Làm tròn các giá trị trong vector random_values thành số nguyên và gán vào vector kết quả. Đồng thời tính tổng các giá trị nguyên này.
        result[i] = static_cast<int>(round(random_values[i]));
        if (result[i] < 1) {
            result[i] = 1; // Đảm bảo rằng tất cả các giá trị là số dương.
        }
        int_sum += result[i];
    }
    int diff = S - int_sum; // Tính hiệu số còn thiếu để tổng của vector kết quả là S.
    // Điều chỉnh các giá trị để đảm bảo tổng là S mà không vượt quá S và tất cả đều là số dương.
    if (diff > 0) {  //nếu tổng còn lại dương-tổng các int chưa bằng triple*3 + single thì tăng các số ngẫu nhiên lên cho đến khi hết tổng đó
        for (int i = 0; i < n && diff > 0; ++i) {
            result[i]++;
            diff--;
        }
    } else { //nếu tổng còn lại âm thì-tổng còn lại vượt quá triple*3 + single thì kiểm tra các số ngẫu nhiên để giảm các số ngẫu nhiên mà chúng không âm cho đến khi hết tổng đó
        for (int i = 0; i < n && diff < 0; ++i) {
            if (result[i] >= 1) {
                result[i]--;
                diff++;
            }
        }
    }
    return result;
}
vector<pair<Ward, int>> WardProcessor(const vector<Ward>& wards, int triple, int single,  int numOfAgents) {
    if((triple+single) != numOfAgents) //nếu tổng single và triple không bằng numOfAgents thì ném ra thông báo lỗi
    throw invalid_argument("Invalid");
    vector<int> random = GetRandom(wards.size()-2,  triple*3 + single); //gọi hàm GetRandom và truyền vào độ dài ward và tổng các int quy định
    vector<pair<Ward, int>> result;
    int remainingTriples = triple; // biến tạm để lưu triple và single còn lại
    int remainingSingles = single;
    for (size_t i = 0; i < wards.size(); ++i) {
        int value;
        if (remainingTriples > 0) {
            value = 3;
            remainingTriples--;
        } else if (remainingSingles > 0) {
            value = 1;
            remainingSingles--;
        } else {
            value = 0; 
        }
        result.push_back(make_pair(wards[i+1], random[i]));
    }
    return result;
}
