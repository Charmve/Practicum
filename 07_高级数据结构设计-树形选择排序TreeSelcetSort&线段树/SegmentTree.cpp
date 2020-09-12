/**********************2.�߶θ������⣨60�֣�***********************
Describe�� 
		����N����������[Ai,Bi]������1<=Ai<Bi<=1000�������ȴ��������С�
	��1��1000��1000������������N�������г��ֹ����Ρ�ע���㷨ʱ�临��
	�ȣ���������жϼ����ķ��������֡�����ʾ���ٶ��߶�����
Author��Zhang Wei 
Time�� 23/12/2017 
*******************************************************************/

#include <iostream> 
#include <stdlib.h>

using namespace std;

#define ERROR 0
#define OK 1 
#define MAXNUM 1000
#define N 1000

struct SegTreeNode
{
    int val;
    int count;
}segTree[MAXNUM];//�����߶������ 

/*******************************************************************
���ܣ������߶���
root����ǰ�߶����ĸ��ڵ��±�
arr: ���������߶���������
qarr�� ����n�����������
n��  ������� 
istart���������ʼλ��
iend������Ľ���λ��
********************************************************************/
void build(int root, int arr[], int qarr[], int n, int istart, int iend){
	//�����߶���
	int i; 
    if(istart == iend)//Ҷ�ӽڵ�
    {
        segTree[root].val = arr[istart];//���ڵ� 
        segTree[root].count = 0;//��ʼ������Ϊ�� 
        for(i=0; i < 2*n ;i+=2){
        	if(qarr[i] < segTree[root].val && segTree[root].val < qarr[i+1]) {
        		segTree[root].count++;
			}	
		}  	
		cout << segTree[root].val <<"'s count is " << segTree[root].count << endl;
	}
		
    else
    {
        int mid = (istart + iend) / 2; 
        build(root*2+1, arr, qarr, n, istart, mid);//�ݹ鹹��������
        build(root*2+2, arr, qarr, n, mid+1, iend);//�ݹ鹹��������
        
        //���������������ڵ��ֵ�����µ�ǰ���ڵ��ֵ
        segTree[root].val = min(segTree[root*2+1].val, segTree[root*2+2].val);
    }
}//build

/*******************************************************************
���ܣ��߶����������ѯ
root����ǰ�߶����ĸ��ڵ��±�
[nstart, nend]: ��ǰ�ڵ�����ʾ������
[qstart, qend]: �˴β�ѯ������
*******************************************************************/
int query(int root, int nstart, int nend, int qstart, int qend)
{
    //��ѯ����͵�ǰ�ڵ�����û�н���
    if(qstart > nend || qend < nstart)
        return ERROR;
    //��ǰ�ڵ���������ڲ�ѯ������
    if(qstart <= nstart && qend >= nend)
        return segTree[root].val;
    //�ֱ������������ѯ���������߲�ѯ����Ľ�Сֵ
    int mid = (nstart + nend) / 2;
    return min(query(root*2, nstart, mid, qstart, qend),
               query(root*2+1, mid + 1, nend, qstart, qend));

}//query

void CreatArr(int a[], int n){
	//�Իس��������룬����n������
	char s;
	int i; 
	for (i = 0; i < 2*n && s != '\n'; i+=2)
	{
		scanf("%d %d",&a[i],&a[i+1]);//��������Ԫ�ر�ʾn�����䷶Χ [1��2]��[3,4]...[i,i+1] 
		s = getchar();
	}
}//CreatArr

int main(void) 
{
	int i,n,arr[N]; 
	printf("��Ҫ���ٸ����䣺"); 
	scanf("%d ",&n);
	CreatArr(arr,n);//n���������� 
	cout<<endl<< "����ѡ��������ǣ�"<<endl; 
	for (i = 0; i < 2*n ; i+=2)
		cout << "[" << arr[i] << ", " << arr[i+1] << "] " ;
	cout << endl << endl;
	
	int Interval_Arr[1000];
	for(i = 0; i < 1000; ) 
		{
			Interval_Arr[i] = ++i;		
		}
	build(0, Interval_Arr, arr, n, 1, 1000);//1~1000����һ���߶���
	
	return 0;	 
} 
