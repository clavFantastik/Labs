#include <iostream>
#include <cmath>


class complex final
{
    private:
        double _real, _im;

    public:
        complex(double real = 0.0, double im = 0.0) : _real(real), _im(im) {};

        void add(const complex value) {
            _real += value._real;
            _im += value._im;
        }

        void sub(const complex value) {
            _real -= value._real;
            _im -= value._im;
        }

        void mult(const complex value) {
            double old_real = _real;
            _real = _real * value._real - _im * value._im;
            _im = old_real * value._im + _im * value._real;
        }

        void div(const complex value) {
            double divider = value._im * value._im + value._real * value._real;
            double old_real = _real;

            if (fabs(divider) < __DBL_EPSILON__) throw std::overflow_error("Divide by zero exception");

            _real = (_real * value._real + _im * value._im) / divider;
            _im = (_im * value._real - old_real * value._im) / divider;
        }

        double argument() const { 
            return atan(_im / _real) * 180 / M_PI; 
        }

        double module() const { 
            return sqrt(_real * _real + _im * _im); 
        }

        void info(std::string label = "") const {
            std::cout << label << this->_real << " + " << this->_im << "i; Argument = " << this->argument() << "°; Module = " << this->module() << std::endl;
        }
};

int main()
{
    //INIT
    complex a(-4, 2);
    complex b(1, -3);
    std::cout << "Init values:" << std::endl;
    a.info(" a = ");
    b.info(" b = ");

    //DIV
    try {
        a.div(b);
        a.info("\nResult of div:\n a = ");
    }
    catch (std::overflow_error const &e) {
        std::cout << e.what() << std::endl;
    }

    //ADD
    a.add(b);
    a.info("\nResult of add:\n a = ");

    //SUB
    a.sub(b);
    a.info("\nResult of sub:\n a = ");

    //MULT
    a.mult(b);
    a.info("\nResult of mult:\n a = ");

    std::cout << std::endl << std::endl;
    
    return 0;
}