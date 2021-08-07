/************************************************************************/
/*对list的实现，采用的是双向链表，C++标准库中使用的也是双向链表 */
/************************************************************************/

#ifndef __EBOXLIST_H
#define  __EBOXLIST_H
#include "stdlib.h"
namespace stl
{
    template <typename Object>
        
    

        
    class List
    {
    public:
                    //定义节点
        struct  Node
        {
            Object data;
            Node* prev;
            Node* next;
            //结构体的构造函数
            Node(Object obj = Object(), Node* p = NULL, Node* n = NULL)
                :data(obj), prev(p), next(n){ }
        };
    //public members
    public:
        //构造函数
        List() : theSize(0)
        {
            init();
        }

        //拷贝构造函数
        List(const List & L)
        {
            //需要深层次的拷贝
            init();
            this->operator=(L);
        }

        //重载赋值运算符
        const List & operator = (List & L)
        {
            if (this == &L)
            {
                return *this;
            }
            
            //形参List是const类型的，因此只能调用const版本的函数
            //将元素一个个拷贝，不拷贝节点，节点在init函数中
            for ( const_iterator from = L.begin();from != L.cend(); ++from)
            {
                push_back(*L);
            }
            return *this;
        }

        bool operator == (const List & L)
        {
            return head == L.head && tail == L.tail && theSize == L.theSize;
        }

        //析构函数
        ~List()
        {
            //删除元素
            Node* temp = head->next;
            Node* tempNext = temp->next;
            while (temp != tail)
            {
                delete temp;
                temp = tempNext;
            }
            //删除头结点和尾节点
            delete head;
            delete tail;
        }

        void init()
        {
            head = new Node;
            tail = new Node;
            head->next = tail;
            tail->prev = head;
        }

    public:
        /************************************************************************/
        /* 说   明：迭代器类的定义*/
        /* 时   间：2016.10.27*/
        /************************************************************************/
        class const_iterator
        {
        public:
            //构造函数，带默认值为NULL
        const_iterator(Node* n = NULL)
                :current(n){ }

            //重载取引用操作符，解引用操作符是无参的
            const Object& operator * ()
            {
                const_iterator::retrieve();
            }

            //重载++，返回的还是迭代器，current发生了改变，不能申明为const
            //返回增加后的引用
            const_iterator & operator ++ ()
            {
                current = current->next;
                return *this;
            }

            //后置，为了与内部保持一致，返回之前的值
            const_iterator & operator ++ (int)
            {
                const_iterator ret = *this;
                ++*this;
                return ret;
            }

            bool operator == (const const_iterator & iter) const
            {
                return current == iter.current;
            }

            bool operator != (const const_iterator & iter) const
            {
                return ! (this->operator== (iter));
            }
            //定义一个Node*的变量来保存迭代器的位置，申明为protected是为了派生类能访问到
            Node* current;    
        protected:


            //返回迭代器所指节点数据对象的引用,不改变const_iterator的任何数据，定义为const
            //该函数只在内部使用，申明为protected为了派生类能使用
            const Object & retrieve()
            {
                return current->data;
            }
//            const_iterator( Node* p ) : current(p) { }

            //为了能访问List<Object>的成员变量，将其申明为const_iterator的友元
            friend class List<Object>;
        };


        //从const_iterator继承过来
        class iterator : public const_iterator
        {
        public:
            iterator();
            iterator(Node* p) : const_iterator (p) { }
            ~iterator(){};
            //重载取引用操作符，解引用操作符是无参的
            Object& operator * () const
            {
                const_iterator::retrieve();
            }

            iterator & operator ++ ()
            {
                const_iterator::current = const_iterator::current->next;
                return *this;
            }

            //后置，为了与内部保持一致，返回之前的值
            iterator & operator ++ (int)
            {
                iterator ret = *this;
                ++*this;
                return ret;
            }
        };

        /************************************************************************/
        /* 函数名：begin end成员*/
        /* 时   间：2016.10.27*/
        /************************************************************************/
        iterator begin()
        {
            return iterator(head->next);
        }

        iterator end()
        {
            return iterator(tail);
        }

        const_iterator cbegin() const
        {
            return const_iterator(head->next);
        }

        const_iterator cend() const
        {
            return const_iterator(tail);
        }


        /************************************************************************/
        /* 说   明：赋值和swap*/
        /* 时   间：2016.10.27*/
        /************************************************************************/

        /************************************************************************/
        /* 说   明：容器大小操作*/
        /* 时   间：2016.10.27*/
        /************************************************************************/
        //size函数
        int Size() const
        {
            return theSize;
        }

        bool Empty() const
        {
            return Size() == 0;
        }



        /************************************************************************/
        /* 说   明：向容器添加元素*/
        /* 时   间：2016.10.27*/
        /************************************************************************/
        void push_back(const Object& obj)
        {
            insert(end(), obj);
        }


        void push_front(Object obj)
        {
            insert(begin(), obj);
        }

        iterator insert(iterator it, const Object &  obj)
        {
            Node* p = it.current;
            Node* node = new Node(obj, p->prev, p);
            p->prev->next = node;
            p->prev = node;
            //大小加一
            ++theSize;
            return iterator(node);
            //牛掰写法：return iterator(p->prev = p->prev->next = new Node(obj, p->prev, p));
        }

        /************************************************************************/
        /* 说   明：访问元素*/
        /* 时   间：2016.10.27*/
        /************************************************************************/
        Object & front() const
        {
            return head->next->data;
        }

        const Object & front()
        {
            return (const Object)(head->next->data);
        }

        Object & back() const
        {
            return tail->prev->data;
        }

        const Object & back() 
        {
            return tail->prev->data;
        }

        /************************************************************************/
        /* 说   明：删除元素*/
        /* 时   间：2016.10.27*/
        /************************************************************************/
        void pop_front()
        {
            Node* temp = head->next;
            temp->next->prev = head;
            head->next = temp->next;
            delete temp;
            theSize--;
        }

        void pop_back()
        {
            erase(--end());
        }

        iterator erase(iterator itr)
        {
            Node* temp = itr.current;
            iterator ret = ++itr;
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
            theSize--;
            return ret;
        }

        iterator erase(iterator itr1, iterator itr2)
        {
            iterator i;
            iterator to  = ++itr2;
            for (i = itr1; i != to; )
            {
                i = erase( i );
            }
            return i;
        }



    private:

        Node* head;   //头结点
        Node* tail;      //尾节点
        int theSize;
    };
}
#endif