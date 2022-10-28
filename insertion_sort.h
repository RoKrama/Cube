#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

namespace isrt
{
template <typename T>
struct Element
{
    Element* nxtp;
    T storedp;

    Element(): nxtp(nullptr) {};

    Element(T &to_str, Element* to_nxt):
        nxtp(to_nxt), storedp(to_str) {};
};

template <typename T>
class List : public Element<T>
{
    Element<T> head, tail;
    bool is_empty = true;
    int call_i =0;

    void insrt(T &insrted, Element<T>* sps)
    {
        if (sps->nxtp->nxtp == nullptr)
        {
            sps->nxtp = new Element<T>(insrted, sps->nxtp);
            return;
        }
        if(insrted >= sps->nxtp->storedp)
            return insrt(insrted, sps->nxtp);

        sps->nxtp = new Element<T>(insrted, sps->nxtp);
        return;
    };

    void call(Element<T> &callee, T array[] )
    {
        array[call_i] = callee.nxtp->storedp;
        call_i++;
        if(callee.nxtp->nxtp->nxtp != nullptr)
            call(*callee.nxtp, array);
        delete callee.nxtp;
    };
    void insrt(T &insrted)
    {
        if(is_empty)
        {
            head.nxtp = new Element<T>(insrted, &tail);
            is_empty = false;
            return;
        }
        insrt(insrted, &head) ;
    };

public:
    List() {};
    List (T in_array[], int size)
    {
        for (int i =0; i<size; i++)
            insrt(in_array[i]);

        call(head, in_array);
    }
};
}
#endif // INSERTION_SORT_H
