/** @file */
#pragma once

#include <vector>
#include <iomanip>

template <typename T>
struct row {
        row(T* pos)
        :pos{pos} { }

        T& operator[](size_t index)
        {
                return pos[index];
        }

        const T& operator[](size_t index) const
        {
                return pos[index];
        }

        T* pos;
};

template <typename T>
class matrix {
public:
        matrix()
        : buff{}, r{0UL}, col{0UL} { }

        matrix(size_t rows, size_t columns)
        : r{rows}, col{columns}
        {
                buff.resize(rows * columns);
        }

        size_t width() const
        {
                // width is the number of columns
                return col;
        }

        size_t height() const
        {
                // height is the number of rows
                return r;
        }

        size_t rows() const
        {
                return r;
        }

        size_t columns() const
        {
                return col;
        }

        std::vector<T>& data()
        {
                return buff;
        }

        const std::vector<T>& data() const
        {
                return buff;
        }

        row<T> operator[](size_t index)
        {
                return row<T>{&buff.data()[width() * index]};
        }

        const row<T> operator[](size_t index) const
        {
                return row<T>{&buff.data()[width() * index]};
        }

private:
        std::vector<T> buff;
        size_t r;
        size_t col;
};

template<typename T>
void print_matrix(matrix<T>& matrix)
{
        std::cout << "-------------------\n";

        std::cout<< "  ";
        for (size_t i = 0; i < matrix.columns(); ++i)
                std::cout << std::setw(5) << i;

        std::cout << "\n\n";

        for (size_t i = 0; i < matrix.rows(); ++i) {
                std::cout << i << ":" << std::setw(2);

                for (size_t j = 0; j < matrix.columns(); ++j)
                        std::cout << std::setw(5) << matrix[i][j];

                std::cout << "\n";
        }

        std::cout << "-------------------\n";
}

