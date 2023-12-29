#pragma once



class Task
{
public:
    Task()
    {}
    Task(int x, int y, char op): _x(x), _y(y), _op(op), _result(0), _resultCode(0)
    {}
    void operator()()
    {
        switch (_op)
        {
        case '+':
            _result = _x + _y;
            break;
        case '-':
            _result = _x - _y;
            break;
        case '*':
            _result = _x * _y;
            break;
        case '/':
            _result = _x / _y;
            break;
        case '%':
            _result = _x % _y;
            break;
        default:
            break;
        }
    }
    string formatArg()
    {
        return to_string(_x) + _op + to_string(_y) + "=";
    }
    string formatRes()
    {
        return to_string(_result) + "(" + to_string(_resultCode) + ")";
    }
    ~Task()
    {}
private:
    int _x;
    int _y;
    char _op;
    int _result;
    int _resultCode;
};