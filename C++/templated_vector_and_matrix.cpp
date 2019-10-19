#include <iostream>

using namespace std;

template<typename T>
class Vector3D
{
public:
    Vector3D()
    {
        x = 0;
        y = 0;
        z = 0;
    };

    Vector3D(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    ~Vector3D() { };

    T getX()
    const {
        return x;
    }

    T getY ()
    const {
        return y;
    }

    T getZ ()
    const {
        return z;
    }

    void setX(T x)
    {
        this->x = x;
    }

    void setY(T y)
    {
        this->y = y;
    }

    void setZ(T z)
    {
        this->z = z;
    }

    bool operator== (const Vector3D<T>& v2)
    {
        return ((this->x == v2.x) && (this->y == v2.y) && (this->z == v2.z));
    }

    bool operator!= (const Vector3D<T>& v2)
    {
        return ((this->x != v2.x) || (this->y != v2.y) || (this->z != v2.z));
    }

    Vector3D operator+ (const Vector3D<T>& v2)
    {
        return Vector3D<T>(this->x + v2.x, this->y + v2.y, this->z + v2.z);
    }

    Vector3D operator- (const Vector3D<T>& v2)
    {
        return Vector3D<T>(this->x - v2.x, this->y - v2.y, this->z - v2.z);
    }

    Vector3D operator* (const T a)
    {
        return Vector3D<T>(this->x * a, this->y * a, this->z * a);
    }

    float operator* (const Vector3D<T>& v2)
    {
        return this->x * v2.x + this->y * v2.y + this->z * v2.z;
    }

protected:
    T x;
    T y;
    T z;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v)
{
      os << "(" << v.getX() << "; " << v.getY() << "; " << v.getZ() << ")" << endl;
      return os;
}

template<typename T>
Vector3D<T> operator* (float a, const Vector3D<T>& v)
{
    return Vector3D<T>(a * v.getX(), a * v.getY(), a * v.getZ());
}

template<typename T>
std::istream& operator>>(std::istream &is, Vector3D<T> &v)
{
    T x, y, z;
    is >> x >> y >> z;
    v.setX(x);
    v.setY(y);
    v.setZ(z);
    return is;
}


template<typename T>
class Matrix3D
{
public:
    Matrix3D()
    {
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
                val[i][j] = 0;
        }
    };

    Matrix3D(T values[3][3])
    {
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
            {
                set_val(i, j, values[i][j]);
            }
        }
    };

    ~Matrix3D() { };

    T get_val(unsigned int i, unsigned int j) const
    {
        if(!(i<0 || i>2 || j<0 || j>2))
            return val[i][j];

        return 0;
    }

    void set_val(unsigned int i, unsigned int j, T value)
    {
        if(!(i<0 || i>2 || j<0 || j>2))
        {
            this->val[i][j] = value;
        }
    }

    bool operator== (const Matrix3D<T>& v2)
    {
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
            {
                if(this->val[i][j] != v2.val[i][j])
                    return false;
            }
        }

        return true;
    }

    bool operator!= (const Matrix3D<T>& v2)
    {
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
            {
                if(this->val[i][j] != v2.val[i][j])
                    return true;
            }
        }

        return false;
    }

    Matrix3D operator+ (const Matrix3D<T>& m2)
    {
        T values[3][3];
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
                values[i][j] = this->val[i][j] + m2.val[i][j];
        }
        return Matrix3D<T>(values);
    }

    Matrix3D operator- (const Matrix3D<T>& m2)
    {
        T values[3][3];
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
                values[i][j] = this->val[i][j] - m2.val[i][j];
        }
        return Matrix3D<T>(values);
    }

    Matrix3D operator* (const float a)
    {
        T values[3][3];
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
                values[i][j] = this->val[i][j] * a;
        }
        return Matrix3D<T>(values);
    }

    Matrix3D operator* (const Matrix3D<T>& m2)
    {
        T values[3][3];
        for(unsigned int i=0; i<3; i++)
        {
            for(unsigned int j=0; j<3; j++)
                values[i][j] = this->val[i][0] * m2.val[0][j] + this->val[i][1] * m2.val[1][j] + this->val[i][2] * m2.val[2][j];
        }
        return Matrix3D<T>(values);
    }

    T det() const
    {
        return val[0][0] * (val[1][1] * val[2][2] - val[1][2] * val[2][1]) - val[0][1] * (val[1][0] * val[2][2] - val[1][2] * val[2][0]) + val[0][2] * (val[1][0] * val[2][1] - val[1][1] * val[2][0]);
    }

    Vector3D<T> operator* (const Vector3D<T>& v)
    {
        T x = val[0][0] * v.getX() + val[0][1] * v.getY() + val[0][2] * v.getZ();
        T y = val[1][0] * v.getX() + val[1][1] * v.getY() + val[1][2] * v.getZ();
        T z = val[2][0] * v.getX() + val[2][1] * v.getY() + val[2][2] * v.getZ();
        return Vector3D<T>(x, y, z);
    }

protected:
    T val[3][3];
};

template<typename T>
Matrix3D<T> operator* (float a, const Matrix3D<T>& m)
{
    T values[3][3];
    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
            values[i][j] = a * m.get_val(i, j);
    }
    return Matrix3D<T>(values);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix3D<T>& m)
{
      os << "||" << m.get_val(0, 0) << " " << m.get_val(0, 1) << " " << m.get_val(0, 2) << "||" << endl;
      os << "||" << m.get_val(1, 0) << " " << m.get_val(1, 1) << " " << m.get_val(1, 2) << "||" << endl;
      os << "||" << m.get_val(2, 0) << " " << m.get_val(2, 1) << " " << m.get_val(2, 2) << "||" << endl;
      return os;
}

template<typename T>
std::istream& operator>>(std::istream &is, Matrix3D<T> &m)
{
    T values[3][3];

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
            is >> values[i][j];
            m.set_val(i, j, values[i][j]);
        }
    }
    return is;
}

int main()
{
    Matrix3D<float> m;
    Vector3D<float> v;

    cin >> m;
    cin >> v;

    cout << m << endl;
    cout << v << endl;

    Vector3D<float> c = m * v;

    cout << c;

    return 0;
}
