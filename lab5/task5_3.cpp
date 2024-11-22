#include <iostream>
#include <stdexcept>
#include <string.h>


class logical_values_array final
{
    private:
        unsigned int _value;

    public:
        logical_values_array(unsigned int value = 0) : _value(value) {} 

        unsigned int accessor() const { 
            return _value; 
        }

        // !
        logical_values_array inversion() const { 
            return logical_values_array(~_value); 
        }

        // &
        logical_values_array conjunction(const logical_values_array &lva) const { 
            return logical_values_array(lva.accessor() & _value); 
        }

        // V == !(!A & !B)
        logical_values_array disjunction(const logical_values_array &lva) const {
            logical_values_array tmp1, tmp2;
            tmp1 = this->inversion();
            tmp2 = lva.inversion();
         
            return tmp1.conjunction(tmp2).inversion();
        }

        // !A V B
        logical_values_array implication(const logical_values_array &lva) const { 
            return this->inversion().disjunction(lva); 
        }

        // A & !B
        logical_values_array coimplication(const logical_values_array &lva) const { 
            return this->conjunction(lva.inversion()); 
        }

        // + == (!A & B) V (A & !B)
        logical_values_array add_mod2(const logical_values_array &lva) const {
            logical_values_array tmp1, tmp2;
            tmp1 = this->inversion().conjunction(lva);
            tmp2 = this->conjunction(lva.inversion());

            return tmp1.disjunction(tmp2);
        }

        // !+
        logical_values_array equal(const logical_values_array &lva) const {
            return this->add_mod2(lva).inversion();
        }

        // !(A V B)
        logical_values_array Pirs(const logical_values_array &lva) const {
            return this->disjunction(lva).inversion();
        }

        // !(A & B)
        logical_values_array Sheffer(const logical_values_array &lva) const {
            return this->conjunction(lva).inversion();
        }

        // ==
        static bool equals(const logical_values_array &lva1, const logical_values_array &lva2) {
            return lva1.accessor() == lva2.accessor();
        }


        bool get_bit(int pos) const {
            if (pos >= sizeof(_value) << 3) throw std::length_error("Error: overflow, incorrect position") ;

            return (1 << pos) & _value;
            
        }

        void get_str_value(char *str_value) {
            if (!str_value) throw std::logic_error("Error: empty string");

            for (int i = 0; i < sizeof(_value) << 3; ++i) {
                str_value[i] = get_bit(i) ? '1' : '0';
                
            }

            str_value[sizeof(_value) << 3] = '\0'; 
        }
};


void short_print_res(logical_values_array operation, std::string label, char *arr) {
    if (!arr) throw std::logic_error("Error: arr is null");

    operation.get_str_value(arr);
    std::cout << label << arr << std::endl;
}

int main()
{
    char arr[128];
    logical_values_array a(500000000);
    logical_values_array b(500000);

    short_print_res(a, "a: ", arr);
    short_print_res(b, "b: ", arr);
    short_print_res(a.inversion(), "inversion a: ", arr);
    short_print_res(a.conjunction(b), "conjunction: ", arr);
    short_print_res(a.disjunction(b), "disjunction: ", arr);
    short_print_res(a.implication(b), "implication: ", arr);
    short_print_res(a.coimplication(b), "coimplication: ", arr);
    short_print_res(a.add_mod2(b), "add_mod2: ", arr);
    short_print_res(a.equal(b), "equal: ", arr);
    short_print_res(a.Pirs(b), "Pirs: ", arr);
    short_print_res(a.Sheffer(b), "Sheffer: ", arr);

    try {
        bool bit = a.get_bit(1);
        std::cout << "a.get_bit(1) = " << bit << std::endl;
        bit = a.get_bit(16);
        std::cout << "a.get_bit(16) = " << bit << std::endl;
    }
    catch (const char *e) {
        std::cout << e << std::endl;
    }

    if (logical_values_array::equals(a, b)) {
        std::cout << "a == b: True" << std::endl;
    }
    else {
        std::cout << "a == b: False" << std::endl;
    }

    return 0;
}