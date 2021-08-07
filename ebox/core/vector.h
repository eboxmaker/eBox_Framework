/************************************************************************/
/* ��vector��ʵ��                                                                     */
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
        /*���캯��*/
        /************************************************************************/
        explicit Vector(int initsize = 0)                     //ѡ��Vector��ʼ��С
            :theSize(initsize)
            ,theCapacity(initsize + SPACE_CAPACITY)
        {
            Objects = new Object[theCapacity];        //�����ʼ�ռ�
        }

        /************************************************************************/
        /*�������캯��*/
        /************************************************************************/
        Vector(const Vector& V)
            :theSize(0)
            ,theCapacity(0)
            ,Objects(NULL)
        {
            *this = V;
        }

        /************************************************************************/
        /*��������                                                                      */
        /************************************************************************/
        ~Vector()
        {
            delete[ ] Objects;
        }

        /************************************************************************/
        /*���ظ�ֵ������                                                                      */
        /************************************************************************/
        Vector &operator = (const Vector& ths)                //��Ҫ�����const���𣿣�������
        {
            if (this != ths)        //��ֹ�뱾��������
            {
                delete [] Objects;         //����ԭ�����ڴ�ռ�
                theSize = ths.theSize;
                theCapacity = ths.theCapacity;
                Objects = new Object[theCapacity];  //���·����ڴ�ռ�
                //�������εĿ���
                /* Objects = ths.Objects  ����Ǳ��εĿ�����ֻ������ָ�룬
                ** ���ԭ����Ķ������ı䣬��ô�������������Ҳ�����ı�*/
                for (int i = 0; i < ths.theSize; ++i)
                {
                    Objects[i] = ths.Objects[i];
                }
            }
            return *this;
        }

        /************************************************************************/
        /* ��������resize*/
        /* ��   �ܣ��ı�size��С*/
        /* ˵   �����ı�theSize�Ĵ�С��theCapacity��С����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        //resize���¶���Ķ������������ֵ��������
        void resize(int newSize)
        {
            if (newSize <= theSize)
                return;
            /*���size����capacity������Ҫ���·����ڴ�ռ�,�ڴ�ռ��С��newsize������*/
            if(newSize > theCapacity)
                reserve(2 * newSize + 1);         //+1��Ϊ�˱�֤newsizeΪ0ʱ��Ϊ�����1�Ŀռ䣬�б�Ҫ�𣿣���
            theSize = newSize;
        }

        /************************************************************************/
        /* ��������reserve*/
        /* ��   �ܣ����·����ڴ�ռ��С*/
        /* ˵   �����ı�theCapacity�Ĵ�С��theSize��С����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        void reserve(int newCapacity)
        {
            if (newCapacity < theSize)
            {
                return;
            }
            /*��ԭ�е����ݿ�������ʳ������*/
            Object * temp = Objects;
            /*Ϊ�������·��������ڴ�ռ�*/
            Objects = new Object[newCapacity];
            /*��ԭ�������ݿ�������,�ڴ�ռ��С�����仯������sizeû�б�*/
            for (int i = 0;  i < theSize; ++i)
            {
                Objects[i] = temp[i];
            }
            /*�ͷ�ԭ���Ŀռ�*/
            delete[] temp;
        }

        /************************************************************************/
        /* ��������operate[]*/
        /* ��   �ܣ������±������*/
        /* ˵   �����ǳ����汾*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        //�±���������ص���Ԫ�ص����ã����峣���汾�ͷǳ����汾
        Object& operator[] (int index)
        {
            return Objects[index];       //ָ����±�����
        }

        /************************************************************************/
        /* ��������operate[]*/
        /* ��   �ܣ������±������*/
        /* ˵   ���������汾*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        const Object& operator[] (int index) const
        {
            return Objects[index];
        }

        /************************************************************************/
        /* ��������isEmpty*/
        /* ��   �ܣ��ж�Vector�Ƿ�Ϊ��*/
        /* ˵   ����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        bool isEmpty() const
        {
            return size() == 0;
        }

        /************************************************************************/
        /* ��������size*/
        /* ��   �ܣ��ж�Vector�Ĵ�С*/
        /* ˵   ����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        int size() const
        {
            return theSize;
        }

        /************************************************************************/
        /* ��������capacity*/
        /* ��   �ܣ��ж�Vector��capacity��С*/
        /* ˵   ����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        int capacity() const
        {
            return theCapacity;
        }

        /************************************************************************/
        /* ��������push_back*/
        /* ��   �ܣ���vector����ѹ��һ������*/
        /* ˵   ����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        void push_back(Object obj)
        {
            //�ж�capacity�Ŀռ��Ƿ���
            if (theSize == theCapacity)
            {
                //����ڴ�ռ䲻����������size����������һ��
                reserve(2 * theCapacity + 1);
            }
            //size��ʾԪ�ص��ܸ����������±��Ǵ�0��ʼ����������±�ȡ����size
            Objects[theSize++] = obj;
        }

        /************************************************************************/
        /* ��������pop_back*/
        /* ��   �ܣ���vector����ɾ��һ������*/
        /* ˵   ����*/
        /* ʱ   �䣺2016.10*/
        /************************************************************************/
        void pop_back()
        {
            --theSize;                  //�ǲ��ǻᵼ�·���Խ�����ֵĽ�����ɿص�ԭ�򣿣�������
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
        Object* Objects;    //vector�ڲ�ʵ�����ǹ�����һ������ָ��,ָ����Ԫ�ص�ָ��
    };
    typedef Vector<int> VectorInt;
}
#endif