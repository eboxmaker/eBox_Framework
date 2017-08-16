
#include "LinearRegression.h"


// �����Իع鷽�̣�Y = a + bx
// dada[rows*2]���飺X, Y��rows������������a, b�����ػع�ϵ��
// SquarePoor[4]�����ط������ָ��: �ع�ƽ���ͣ�ʣ��ƽ���ͣ��ع�ƽ���ʣ��ƽ����
// ����ֵ��0���ɹ���-1����

    
int linear_regression(double *data, int rows, double *a, double *b, double *SquarePoor)
{
    int m;
    double *p, Lxx = 0.0, Lxy = 0.0, xa = 0.0, ya = 0.0;
    if (data == 0 || a == 0 || b == 0 || rows < 1)
        return -1;
    for (p = data, m = 0; m < rows; m ++)
    {
        xa += *p ++;
        ya += *p ++;
    }
    xa /= rows;                                     // Xƽ��ֵ
    ya /= rows;                                     // Yƽ��ֵ
    for (p = data, m = 0; m < rows; m ++, p += 2)
    {
        Lxx += ((*p - xa) * (*p - xa));             // Lxx = Sum((X - Xa)ƽ��)
        Lxy += ((*p - xa) * (*(p + 1) - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    }
    *b = Lxy / Lxx;                                 // b = Lxy / Lxx
    *a = ya - *b * xa;                              // a = Ya - b*Xa
    if (SquarePoor == 0)
        return 0;
    // �������
    SquarePoor[0] = SquarePoor[1] = 0.0;
    for (p = data, m = 0; m < rows; m ++, p ++)
    {
        Lxy = *a + *b * *p ++;
        SquarePoor[0] += ((Lxy - ya) * (Lxy - ya)); // U(�ع�ƽ����)
        SquarePoor[1] += ((*p - Lxy) * (*p - Lxy)); // Q(ʣ��ƽ����)
    }
    SquarePoor[2] = SquarePoor[0];                  // �ع鷽��
    SquarePoor[3] = SquarePoor[1] / (rows - 2);     // ʣ�෽��
    return 0;
}

void FreeData(double **dat, double *d, int count)
{
    int i, j;
    free(d);
    for (i = 0; i < count; i ++)
        free(dat[i]);
    free(dat);
}
// �����Է��̡�data[count*(count+1)]�������飻count������Ԫ����
// Answer[count]��������� �����أ�0���ɹ���-1�޽���������
int LinearEquations(double *data, int count, double *Answer)
{
    int j, m, n;
    double tmp, **dat, *d = data;
    dat = (double**)ebox_malloc(count * sizeof(double*));
    for (m = 0; m < count; m ++, d += (count + 1))
    {
        dat[m] = (double*)malloc((count + 1) * sizeof(double));
        memcpy(dat[m], d, (count + 1) * sizeof(double));
    }
    d = (double*)malloc((count + 1) * sizeof(double));
    for (m = 0; m < count - 1; m ++)
    {
        // ������Խ���Ԫ��Ϊ0���н���
        for (n = m + 1; n < count && dat[m][m] == 0.0; n ++)
        {
            if ( dat[n][m] != 0.0)
            {
                memcpy(d, dat[m], (count + 1) * sizeof(double));
                memcpy(dat[m], dat[n], (count + 1) * sizeof(double));
                memcpy(dat[n], d, (count + 1) * sizeof(double));
            }
        }
        // �н��������Խ���Ԫ����ȻΪ0���޽⣬����-1
        if (dat[m][m] == 0.0)
        {
            FreeData(dat, d, count);
            return -1;
        }
        // ��Ԫ
        for (n = m + 1; n < count; n ++)
        {
            tmp = dat[n][m] / dat[m][m];
            for (j = m; j <= count; j ++)
                dat[n][j] -= tmp * dat[m][j];
        }
    }
    for (j = 0; j < count; j ++)
        d[j] = 0.0;
    // ���count - 1��Ԫ
    Answer[count - 1] = dat[count - 1][count] / dat[count - 1][count - 1];
    // ���д������Ԫ
    for (m = count - 2; m >= 0; m --)
    {
        for (j = count - 1; j > m; j --)
            d[m] += Answer[j] * dat[m][j];
        Answer[m] = (dat[m][count] - d[m]) / dat[m][m];
    }
    FreeData(dat, d, count);
    return 0;
}

// ���Ԫ�ع鷽�̣�Y = B0 + B1X1 + B2X2 + ...BnXn
// data[rows*cols]��ά���飻X1i,X2i,...Xni,Yi (i=0 to rows-1)
// rows������������cols����������Answer[cols]�����ػع�ϵ������(B0,B1...Bn)
// SquarePoor[4]�����ط������ָ��: �ع�ƽ���ͣ�ʣ��ƽ���ͣ��ع�ƽ���ʣ��ƽ����
// ����ֵ��0���ɹ���-1����
int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor)
{
    int m, n, i, count = cols - 1;
    double *dat, *p, a, b;
    if (data == 0 || Answer == 0 || rows < 2 || cols < 2)
        return -1;
    dat = (double*)malloc(cols * (cols + 1) * sizeof(double));
    dat[0] = (double)rows;
    for (n = 0; n < count; n ++)                     // n = 0 to cols - 2
    {
        a = b = 0.0;
        for (p = data + n, m = 0; m < rows; m ++, p += cols)
        {
            a += *p;
            b += (*p * *p);
        }
        dat[n + 1] = a;                              // dat[0, n+1] = Sum(Xn)
        dat[(n + 1) * (cols + 1)] = a;               // dat[n+1, 0] = Sum(Xn)
        dat[(n + 1) * (cols + 1) + n + 1] = b;       // dat[n+1,n+1] = Sum(Xn * Xn)
        for (i = n + 1; i < count; i ++)             // i = n+1 to cols - 2
        {
            for (a = 0.0, p = data, m = 0; m < rows; m ++, p += cols)
                a += (p[n] * p[i]);
            dat[(n + 1) * (cols + 1) + i + 1] = a;   // dat[n+1, i+1] = Sum(Xn * Xi)
            dat[(i + 1) * (cols + 1) + n + 1] = a;   // dat[i+1, n+1] = Sum(Xn * Xi)
        }
    }
    for (b = 0.0, m = 0, p = data + n; m < rows; m ++, p += cols)
        b += *p;
    dat[cols] = b;                                   // dat[0, cols] = Sum(Y)
    for (n = 0; n < count; n ++)
    {
        for (a = 0.0, p = data, m = 0; m < rows; m ++, p += cols)
            a += (p[n] * p[count]);
        dat[(n + 1) * (cols + 1) + cols] = a;        // dat[n+1, cols] = Sum(Xn * Y)
    }
    n = LinearEquations(dat, cols, Answer);          // ���㷽��ʽ
    // �������
    if (n == 0 && SquarePoor)
    {
        b = b / rows;                                // b = Y��ƽ��ֵ
        SquarePoor[0] = SquarePoor[1] = 0.0;
        p = data;
        for (m = 0; m < rows; m ++, p ++)
        {
            for (i = 1, a = Answer[0]; i < cols; i ++, p ++)
                a += (*p * Answer[i]);               // a = Ym�Ĺ���ֵ
            SquarePoor[0] += ((a - b) * (a - b));    // U(�ع�ƽ����)
            SquarePoor[1] += ((*p - a) * (*p - a));  // Q(ʣ��ƽ����)(*p = Ym)
        }
        SquarePoor[2] = SquarePoor[0] / count;       // �ع鷽��
  if (rows - cols > 0.0)
    SquarePoor[3] = SquarePoor[1] / (rows - cols); // ʣ�෽��
  else
    SquarePoor[3] = 0.0;
    }
    free(dat);
    return n;
}




































/*ʹ�÷���
#include "ebox.h"
#include "math.h"
#include "../core/math/LinearRegression.h"
double data1[12][2] = {
//    X      Y
    {187.1, 25.4},
    {179.5, 22.8},
    {157.0, 20.6},
    {197.0, 21.8},
    {239.4, 32.4},
    {217.8, 24.4},
    {227.1, 29.3},
    {233.4, 27.9},
    {242.0, 27.8},
    {251.9, 34.2},
    {230.0, 29.2},
    {271.8, 30.0}
};
double answer[2];
double SquarePoor[4];


void Display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
{
    double v, *p;
    int i, j;
    uart1.printf("�ع鷽��ʽ:    Y = %.5lf", Answer[0]);
    for (i = 1; i < cols; i ++)
        uart1.printf(" + %.5lf*X%d\r\n", Answer[i], i);
    uart1.printf("�ع������Լ���: \r\n");
    uart1.printf("�ع�ƽ���ͣ�%12.4lf  �ع鷽�%12.4lf \r\n", SquarePoor[0], SquarePoor[2]);
    uart1.printf("ʣ��ƽ���ͣ�%12.4lf  ʣ�෽�%12.4lf \r\n", SquarePoor[1], SquarePoor[3]);
    uart1.printf("���ƽ���ͣ�%12.4lf  ��׼��%12.4lf \r\n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    uart1.printf("F   ��  �飺%12.4lf  ���ϵ����%12.4lf \r\n", SquarePoor[2] /SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    uart1.printf("ʣ�����: \r\n");
    uart1.printf("      �۲�ֵ      ����ֵ      ʣ��ֵ    ʣ��ƽ�� \r\n");
    for (i = 0, p = dat; i < rows; i ++, p ++)
    {
        v = Answer[0];
        for (j = 1; j < cols; j ++, p ++)
            v += *p * Answer[j];
        uart1.printf("%12.2lf%12.2lf%12.2lf%12.2lf \r\n", *p, v, *p - v, (*p - v) * (*p - v));
    }
}


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");

    
    LinearRegression((double*)data1,12,&answer[0],&answer[1],&SquarePoor[0]);
    Display((double*)data1,answer,&SquarePoor[0],12,2);
}

int main(void)
{
    setup();

    while(1)
    {

    }
}
*/