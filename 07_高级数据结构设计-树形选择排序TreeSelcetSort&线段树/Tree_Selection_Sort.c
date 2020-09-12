/**********************1.�����������㷨��40�֣�***********************
Describe�� 
	������д����ʵ������ѡ������Ҫ��ʵ�����������������
Author��Zhang Wei 
Time�� 15/12/2017 
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 99
#define N 100

/**   
     * ---------------------------------------   
     * ����ѡ������ ��  
     * ���ڼ�������˵����Ҫ�ǽ���n-1��Ԫ�صıȽϣ�ÿ�˱Ƚ�n-2�Σ�  
     * ÿ�˱Ƚ�ȡ��һ����Сֵ(Ҳ���������ֵ)����ʹ�б�����  
     * ���ǵ�һ�˵ıȽ��ǿ���Ϊ�����ıȽ��ṩ��Ϣ�ģ�ʹ�����ıȽϴ��������٣�  
     * �������ıȽ��ֿ���Ϊ�������ıȽ��ṩ��Ϣ�������ͼ����˱ȽϵĴ�����������  
     * ʱ�临�Ӷȡ�  
     *   
     * ʵ��ԭ��  
     * ��һ�������ȶ�n����¼���������Ƚϣ��õ���С��n/2���������αȽϣ���������  
     * ֱ���õ�һ����Сֵ,����һ��������ȫ�������Ĺ��̣����ڵ㼴Ϊ��СԪ�أ�Ҷ�ӽڵ�Ϊ�б�Ԫ�ء�  
     * ����Ĵ����Ĵ洢�ṹ�����������ʾ���������鳤��Ϊ2n-1��������������  
     * �б�Ԫ��Ϊ��19    38     65    97   76    13    27   49  
     * �������Ϊ��                13  
     *                     38               13  
     *                38       65       13       27  
     *              19  38   65  97   76  13   27  49  
     * 13Ϊ�����λ��Сֵ���б�Ԫ��ΪҶ�ӽڵ�  
     *   
     * �ڶ�����������СԪ�أ���ʱ������Ϊ����a�ĵ�һ��λ�ø�ֵΪ����Сֵ��  
     * ֮������ҳ���Сֵ�����Ϊ�ڶ���λ�ø�ֵ��......  
     *   
     * ���������ҳ���Сֵ���ҳ���Сֵ��Ҷ�ӽڵ��λ�ã��Ӹýڵ㿪ʼ�������ֵܽڵ�  
     * ���бȽϣ��޸Ĵ�Ҷ�ӽڵ㵽���ڵ��Ԫ��ֵ���Ƚ���Ϻ󣬸��ڵ�Ϊ��Сֵ��  
     * �������Ƚ��������˵�һ�αȽ��ṩ����Ϣ����Ϊ��һ���Ѿ��õ��������Ƚϵ�  
     * ��Сֵ��ֻҪ�õ�һ������Сֵ�Ƚϵ�Ԫ��(����Сֵ���ֵܽڵ�)�����ǱȽϼ��ɵ���Сֵ��  
     * �����������ӵ�76��27�Ƚϣ�Ȼ��27��38�Ƚϵõ���Сֵ27��  
     * �ظ��ڶ��͵�������������ɡ�  
     *   
     * PS:������Ƴ�ȥ��Ҷ�ӽڵ㶼Ҫ����Ϊ���ֵ���ɶԴ˷���������΢�Ķ�  
     * �ɴ�һ�����ֵ������������������������Integer.MAX_VALUE  
     */  
	     
int TreeSelectionSort(int mData[],int len) 
{  
    int TreeLong = len * 4;  
    int MinValue = -10000;  
    int tree[TreeLong]; // ���Ĵ�С  
    int baseSize;  
    int i;  
    int max;  
    int maxIndex;  
    int treeSize;  
    baseSize = 1;  
      
    while (baseSize < len)   
        baseSize *= 2;   
    treeSize = baseSize * 2 - 1;  
      
    for (i = 0; i < len; i++)   
        tree[treeSize - i] = mData[i];  
      
    for (; i < baseSize; i++)  
        tree[treeSize - i] = MinValue;  
      
    // ����һ����  
    for (i = treeSize; i > 1; i -= 2) {  
        tree[i / 2] = (tree[i] > tree[i - 1] ? tree[i] : tree[i - 1]);  
    }  
    
    len -= 1;  
    while (len != -1) {  
        max = tree[1];  
        mData[len--] = max;  
        maxIndex = treeSize;  
        while (tree[maxIndex] != max)  
            maxIndex--;  
          
        tree[maxIndex] = MinValue;  
        while (maxIndex > 1) {  
            if (maxIndex % 2 == 0) {  
                tree[maxIndex / 2] = (tree[maxIndex] > tree[maxIndex + 1] ? tree[maxIndex]  
                        : tree[maxIndex + 1]);  
            } 
			else {  
                tree[maxIndex / 2] = (tree[maxIndex] > tree[maxIndex - 1] ? tree[maxIndex]  
                        : tree[maxIndex - 1]);  
            }  
            maxIndex /= 2;  
        }  
    }  
    //return mData;  
}

/*void TreeSelectionSort(int a[],int len)
{  
    int treeSize = 2 * len - 1;  //��ȫ�������Ľڵ���    
    int low = 0;
	int i,j;    
    int tree[treeSize];    //��ʱ�����洢�ռ�    
    //�ɺ���ǰ��������������0��ʼ    
    for(i = len-1,j=0 ;i >= 0; --i,j++)
	    //���Ҷ�ӽڵ�    
        tree[treeSize-1-j] = a[i];        
        
    for(j = treeSize-1;j>0;j-=2) 
		//�����ն˽ڵ�    
        tree[(j-1)/2] = (tree[j-1] < tree[j] ? tree[j-1]:tree[j]);        
        
    //����������С�ڵ�    
    int minIndex;    
    while(low < len){    
        int min = tree[0];    //��Сֵ    
        a[low++] = min;    
        minIndex = treeSize-1;       //�ҵ���Сֵ������    
        while(tree[minIndex] != min)    
            minIndex--;    
       
        tree[minIndex] = MAX_VALUE;  //����һ�����ֵ��־    
        //�ҵ����ֵܽڵ�    
        while(minIndex > 0){      //����仹�и��ڵ�    
            if(minIndex % 2 == 0){   //������ҽڵ�    
                tree[(minIndex-1)/2] = (tree[minIndex-1] < tree[minIndex]) 
				? tree[minIndex-1]:tree[minIndex];    
                minIndex = (minIndex-1)/2;    
            }
			else{                   //�������ڵ�    
                tree[minIndex / 2] = (tree[minIndex] < tree[minIndex+1])    
                 ? tree[minIndex]:tree[minIndex+1];    
                 minIndex = minIndex/2;    
            }    
        }    
            
    }    
}    
*/

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int CreatArr(int a[]){
	//�Իس��������룬����һ������������ 
	char s;
	int i,c = 0; 
	for (i = 0; i < N && s != '\n';i++)
	{
		scanf("%d",&a[i]);
		s = getchar();
		c++;
	}
	return c;
}

	//���������ʽ��
	//����: n arr(1),arr(2),arr(3)...arr(n)
	//����� Orignial array:
	//       arr(1),arr(2),arr(3)...arr(n)
	//       Sorted array:
	//       .... 
	/*
			2
			3 4 6 2 5 7 10
			Orignial array:
			3 4 6 2 5 7 10
			Sorted array:
			2 3 4 5 6 7 10
			
			109 34 27 238 35 94
			Orignial array:
			109 34 27 238 35 94
			Sorted array:
			27 34 35 94 109 238 
			
    */

void main() 
{  
	// TODO Auto-generated method stub  
	//int arr[] = { 38, 62, 35, 77, 55, 14, 35, 98 };
	int n,l,j;
	int arr[N];
	
	scanf("%d ",&n); //���Դ��� 
	for(j = 0; j < n; j++)
	{
		l = CreatArr(arr);	
		//int l = sizeof(arr)/sizeof(arr[0]);
	
		printf("Orignial array: \n");
		printArray(arr, l);
		
		TreeSelectionSort(arr,l);  
	    printf("Sorted array: \n");
	    printArray(arr, l);
		
		printf("\n");  
	}
}  

