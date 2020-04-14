#include <iostream>
#include <string>
#include <vector>

class FindSubMatrix{
public:
    FindSubMatrix(std::vector<std::vector<int>>& _matrix, std::vector<std::vector<int>>& _sub_matrix);

    std::pair<int, int> get_sub_matrix_pos();
    // 0 - North, 1 - NorthEast, 2 ...
    int get_sub_matrix_direction();

private:
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> sub_matrix;

    std::vector<std::vector<long long>> matrix_hash;
    std::vector<long long> sub_matrix_hash;

    long long pow[1000];

    std::vector<std::vector<long long>> polinom_matrix_hash(const std::string& matrix);
    std::vector<long long> polinom_sub_matrix_hash(const std::string& matrix);

};

FindSubMatrix::FindSubMatrix(std::vector<std::vector<int>>& _matrix, std::vector<std::vector<int>>& _sub_matrix):
    matrix(_matrix),
    sub_matrix(_sub_matrix)
{
    // prepare matrix hash
    std::string string_matrix;
    int n = matrix.size(); // length of matrix
    for(int i=0; i < n; ++i)
        for(int j=0; j < n; ++j)
            string_matrix[i*n+j] = matrix[i][j];
    matrix_hash = polinom_matrix_hash(string_matrix);

    // prepare sub_matrix hash
    std::string sub_string_matrix;
    int m = sub_matrix.size(); // length of sub matrix
    for(int i=0; i < m; ++i)
        for(int j=0; j < m; ++j)
            sub_string_matrix[i*m+j] = sub_matrix[i][j];
    sub_matrix_hash = polinom_sub_matrix_hash(sub_string_matrix);

    int parameter = 257;
    pow[0] = 1;
    for (int i = 1; i <= 1000; i++) {
        pow[i] = pow[i - 1] * parameter % 100000000000007;
    }
}

std::pair<int, int> FindSubMatrix::get_sub_matrix_pos(){
    std::pair<int, int> result_position;

    int n = matrix.size();
    int m = sub_matrix.size();
    std::vector<std::vector<int>> count_of_equal_lines;   // if value == m it mean we found sub_matrix
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            count_of_equal_lines[i][j] = 0;

    for(int i=0; i < n; ++i){
        for (int j = 0; j < n - m; ++j) {
            for (int k = 0; k < m; ++k) {
                if(matrix_hash[i][j + k] == pow[i*(j+k)*n]*sub_matrix_hash[i])
                {
                    if(i == 0)
                        count_of_equal_lines[i][j+k] = 1;       // first common line
                    else
                    count_of_equal_lines[i][j+k] = count_of_equal_lines[i-1][j+k] + 1;  // next common line
                }
                else{
                    break;
                }
            }
        }
    }

    result_position = std::make_pair(-1, -1);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (count_of_equal_lines[i][j] == m)
                result_position = std::make_pair(i - m, j);

    return result_position;
}

int FindSubMatrix::get_sub_matrix_direction(){
    /// ...
}

std::vector<std::vector<long long>> FindSubMatrix::polinom_matrix_hash(const std::string& matrix) {
    int len_of_string_in_matrix = matrix.size();
    int len_of_raw_in_matrix = matrix.size(); // square
    int size_of_searching_string_matrix = sub_matrix.size();
    std::vector<std::vector<long long>> result(len_of_raw_in_matrix,
                                     std::vector<long long>(len_of_string_in_matrix - size_of_searching_string_matrix,0));

    std::vector<long long> prefix;

    prefix.assign(matrix.length(), 0);
    // calculate hash of prefix
    prefix[0] = ((int)matrix[0] - 48);
    for (int i = 1; i < matrix.length(); i++) {
        prefix[i] = prefix[i - 1] + pow[i] * ((int)matrix[i] - 48);
    }

    for (int i = 0; i < len_of_raw_in_matrix; ++i) {
        result[i][0] = prefix[i*len_of_raw_in_matrix + size_of_searching_string_matrix - 1];
        for (int j = 1; j < len_of_string_in_matrix - size_of_searching_string_matrix; ++j) {
            result[i][j] = prefix[i*len_of_raw_in_matrix + j + size_of_searching_string_matrix - 1]
                           - prefix[i*len_of_raw_in_matrix + j - 1];
        }
    }
    return result;
}

std::vector<long long> FindSubMatrix::polinom_sub_matrix_hash(const std::string& matrix) {
    int len_of_string = sub_matrix.size();
    int len_of_raw = sub_matrix.size();     // square
    std::vector<long long> result(len_of_raw, 0);

    for (int j = 0; j < len_of_raw; ++j) {
        long long current_hash = ((int)matrix[0] - 48);
        for (int i = 1; i < len_of_string; i++) {
            current_hash = current_hash + pow[i] * ((int)matrix[i] - 48);
        }
        result[j] = current_hash;
    }
    return result;
}


int main() {
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> sub_matrix;

    FindSubMatrix find_sub_matrix(matrix, sub_matrix);

    std::cout << find_sub_matrix.get_sub_matrix_pos().first << ' ' << find_sub_matrix.get_sub_matrix_pos().second << std::endl ;
    // 0 - North, 1 - NorthEast, 2 ...
    std::cout << find_sub_matrix.get_sub_matrix_direction() << std::endl;

    return 0;
}