/************************************************************************/
/* 对vector的实现                                                                     */
/************************************************************************/
#ifndef VECTOR_H
#define VECTOR_H
#include "string.h"
#define  SPACE_CAPACITY 5
namespace stl
{
    template<typename Object>
    class Vector
    {
    public:
        /************************************************************************/
        /*构造函数*/
        /************************************************************************/
        explicit Vector(int initsize = 0)                     //选择Vector初始大小
            :theSize(initsize)
            ,theCapacity(initsize + SPACE_CAPACITY)
        {
            Objects = new Object[theCapacity];        //分配初始空间
        }

        /************************************************************************/
        /*拷贝构造函数*/
        /************************************************************************/
        Vector(const Vector& V)
            :theSize(0)
            ,theCapacity(0)
            ,Objects(NULL)
        {
            *this = V;
        }

        /************************************************************************/
        /*析构函数                                                                      */
        /************************************************************************/
        ~Vector()
        {
            delete[ ] Objects;
        }

        /************************************************************************/
        /*重载赋值操作符                                                                      */
        /************************************************************************/
        Vector &operator = (const Vector& ths)                //需要定义成const型吗？？？？？
        {
            if (this != ths)        //防止与本身发生拷贝
            {
                delete [] Objects;         //销毁原来的内存空间
                theSize = ths.theSize;
                theCapacity = ths.theCapacity;
                Objects = new Object[theCapacity];  //重新分配内存空间
                //进行深层次的拷贝
                /* Objects = ths.Objects  这是潜层次的拷贝，只拷贝了指针，
                ** 如果原对象的对象发生改变，那么拷贝对象的内容也发生改变*/
                for (int i = 0; i < ths.theSize; ++i)
                {
                    Objects[i] = ths.Objects[i];
                }
            }
            return *this;
        }

        /************************************************************************/
        /* 函数名：resize*/
        /* 功   能：改变size大小*/
        /* 说   明：改变theSize的大小，theCapacity大小不变*/
        /* 时   间：2016.10*/
        /************************************************************************/
        //resize后新定义的对象会怎样被赋值？？？？
        void resize(int newSize)
        {
            if (newSize <= theSize)
                return;
            /*如果size大于capacity，则需要重新分配内存空间,内存空间大小是newsize的两倍*/
            if(newSize > theCapacity)
                reserve(2 * newSize + 1);         //+1是为了保证newsize为0时，为其分配1的空间，有必要吗？？？
            theSize = newSize;
        }

        /************************************************************************/
        /* 函数名：reserve*/
        /* 功   能：重新分配内存空间大小*/
        /* 说   明：改变theCapacity的大小，theSize大小不变*/
        /* 时   间：2016.10*/
        /************************************************************************/
        void reserve(int newCapacity)
        {
            if (newCapacity < theSize)
            {
                return;
            }
            /*把原有的内容拷贝到零食数组中*/
            Object * temp = Objects;
            /*为对象重新分配更大的内存空间*/
            Objects = new Object[newCapacity];
            /*把原来的数据拷贝回来,内存空间大小发生变化，但是size没有变*/
            for (int i = 0;  i < theSize; ++i)
            {
                Objects[i] = temp[i];
            }
            /*释放原来的空间*/
            delete[] temp;
        }

        /************************************************************************/
        /* 函数名：operate[]*/
        /* 功   能：重载下标操作符*/
        /* 说   明：非常量版本*/
        /* 时   间：2016.10*/
        /************************************************************************/
        //下标运算符返回的是元素的引用，定义常量版本和非常量版本
        Object& operator[] (int index)
        {
            return Objects[index];       //指针的下标运算
        }

        /************************************************************************/
        /* 函数名：operate[]*/
        /* 功   能：重载下标操作符*/
        /* 说   明：常量版本*/
        /* 时   间：2016.10*/
        /************************************************************************/
        const Object& operator[] (int index) const
        {
            return Objects[index];
        }

        /************************************************************************/
        /* 函数名：isEmpty*/
        /* 功   能：判断Vector是否为空*/
        /* 说   明：*/
        /* 时   间：2016.10*/
        /************************************************************************/
        bool isEmpty() const
        {
            return size() == 0;
        }

        /************************************************************************/
        /* 函数名：size*/
        /* 功   能：判断Vector的大小*/
        /* 说   明：*/
        /* 时   间：2016.10*/
        /************************************************************************/
        int size() const
        {
            return theSize;
        }

        /************************************************************************/
        /* 函数名：capacity*/
        /* 功   能：判断Vector的capacity大小*/
        /* 说   明：*/
        /* 时   间：2016.10*/
        /************************************************************************/
        int capacity() const
        {
            return theCapacity;
        }

        /************************************************************************/
        /* 函数名：push_back*/
        /* 功   能：在vector后面压入一个数据*/
        /* 说   明：*/
        /* 时   间：2016.10*/
        /************************************************************************/
        void push_back(Object obj)
        {
            //判断capacity的空间是否还有
            if (theSize == theCapacity)
            {
                //如果内存空间不够，在现有size基础上扩容一倍
                reserve(2 * theCapacity + 1);
            }
            //size表示元素的总个数，但是下标是从0开始计数，因此下标取不到size
            Objects[theSize++] = obj;
        }

        /************************************************************************/
        /* 函数名：pop_back*/
        /* 功   能：在vector后面删除一个数据*/
        /* 说   明：*/
        /* 时   间：2016.10*/
        /************************************************************************/
        void pop_back()
        {
            --theSize;                  //是不是会导致访问越界后出现的结果不可控的原因？？？？？
        }

        const Object & back() const
        {
            return Objects[size() - 1];
        }

        typedef Object* iterator;
        typedef const Object* const_iterator;

        iterator begin()
        {
            return &Objects[0];
        }

        iterator end()
        {
            return &Objects[size()];
        }

        const_iterator cbegin() const
        {
            return &Objects[0];
        }

        const_iterator cend() const
        {
            return &Objects[size()];
        }
        void clear()
        {
            theSize = 0;
        }
        iterator erase(iterator position) 
        {
            int startIndex = 0;
            if (position + 1 != end())
            {
                for(int i = 0; i < theSize;i++)
                {
                    if(&Objects[i] == position)
                    {
                        startIndex = i;
                        break;
                    }
                }
                for(int i = startIndex; i < theSize - 1;i++)
                {
                    Objects[i] = Objects[i+1];
                }
            }
            --theSize;
            return position;
        }
    private:
        int theSize;
        int theCapacity;
        Object* Objects;    //vector内部实际上是管理了一个数组指针,指向首元素的指针
    };
    typedef Vector<int> VectorInt;
}
#endif