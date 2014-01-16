#include<stdio.h>
#include<stdlib.h>
#include<string.h>



#define inputSize  4002412	/*�z�肳���W�����͂̍ő�T�C�Y*/


int read_int( char *start, char **stop );		/*���͂̓ǂݎ�荂�����̂��߁A�W�����͂̃o�b�t�@����擪�̐�����ϊ����Ĕ����o��*/								
void distsort(int n, const int a[], int b[]);	/*���z�����\�[�g(�l�b�g����R�s�y)*/
int searchMax(int target,int *prices,			/*�T���@�\�{��*/
			  int *diagonal_nums,int pN);
int binary_UnderMaxSearch(int *diagonal_nums,	/*diagonal_nums�̒�����A<=len_diagonal_nums�̒l��2���T���ɂ���ĒT������	*/
						  int len_diagonal_nums,
						 int target);


int main(){						   
	
	int pN=0;				/*���i�̑���*/
	int dN=0;				/*�L�����y�[���̓���*/						
	
	int *prices_before;		/*�\�[�g�O�̏��i���i�̔z��*/
	int *prices;			/*�\�[�g�ς݂̏��i���i�̔z��*/
	int *target;			/*��薈�̖ڕW���i�̔z��*/

	int *diagonal_nums;		/*�T���A���S���Y���ɂāA���i�\���΂߂̏���ۑ�����z��*/
	
	char *raw_input;		/*���͂̓ǂݎ�荂�����̂��߁A�W�����͂�fread�������ʂ��i�[����z��*/				
	char *top;				/*���͂̓ǂݎ�荂�����̂��߂�fp(�W�����͂̎��̃X�L�����ʒu���i�[)*/								
	int i,m,n;
	
	/*�z�肳���ő���̓T�C�Y����A�X�^�b�N�̈�ł̗e�ʊm�ۂ��ł��Ȃ��������߁A
	�q�[�v�̈�Ŋm�ۂ���*/
	raw_input = (char *)malloc(inputSize* sizeof(char));;

	fread(raw_input,sizeof(char)*inputSize,1,stdin);

	pN = read_int(raw_input,&top);
    dN = read_int(top,&top);
    
	prices_before = (int *)malloc(pN * sizeof(int));
	prices = (int *)malloc(pN * sizeof(int));
	target = (int *)malloc(dN * sizeof(int));
	
	for(n=0;n<pN;n++){
		prices_before[n] = read_int(top , &top);
	}

	for(n=0;n<dN;n++){
		target[n]= read_int(top, &top);	
	}

	distsort(pN,prices_before,prices);

	diagonal_nums = (int *)malloc((pN-1)* sizeof(int));
	for(i=1;i<pN;i++){
		diagonal_nums[i-1] = prices[i] + prices[i-1];
	}

	 //����r����
	 for(m=0;m<dN;m++){		 
		 printf("%d\n",searchMax(target[m],prices,diagonal_nums,pN));
	 }

	free(raw_input);
	free(prices_before);
	free(prices);
	free(target);
	free(diagonal_nums);
	
	return 0;
}

	


/*�ȉ��̕ϐ���read_int��p�̂��̂����A�Ăяo���񐔂����ɑ���(�ő�5����)���߁A
�ϐ��̐錾�R�X�g��ߖ��ړI�Ƃ��ăO���[�o��������*/				

int STDR_word;		/*start����ꕶ������char���擾���Aint�ɕϊ������l���i�[*/		

char *STDR_nums;	/*�����Ɗm�肵��������ւ̓ǂݏo���ɗ��p����|�C���^*/
int STDR_len=0;		/*�����Ɗm�肵��������̒���*/		
int STDR_readPoint;	/*�����Ɗm�肵��������̓��A�Ō������1�����������Ă����|�C���^*/   
int STDR_result=0;	/*�ŏI�I�ɒ��o����鐔�����i�[����*/

int DecimalWeight[] = {1,10,100,1000,10000,100000,1000000,10000000};

int read_int( char *start, char **stop )
{
	char *end= start;	
	
	do{
		STDR_word = (int)(*++end);
	}while (48<=STDR_word && STDR_word <=57);
	
	STDR_nums = end-1;
	STDR_len = STDR_nums- start;
	STDR_readPoint = STDR_len+1;

	while(STDR_readPoint--){
		STDR_result+= (*(STDR_nums--)-48)*DecimalWeight[STDR_len - STDR_readPoint];
	};

	*stop = end+1;
    return STDR_result;
}


#define MAX 1000000
#define MIN 10

void distsort(int n, const int a[], int b[])
{
    int i, x;
    static int count[MAX - MIN + 1];

    for (i = 0; i <= MAX - MIN; i++) count[i] = 0;
    for (i = 0; i < n; i++) count[a[i] - MIN]++;
    for (i = 1; i <= MAX - MIN; i++) count[i] += count[i - 1];
    for (i = n - 1; i >= 0; i--) {
        x = a[i] - MIN;  b[--count[x]] = a[i];
    }
}


int searchMax(int target,int *prices,int *diagonal_nums,int pN){
		
	int maxValue=0;
	int x,y,ly,lx;
	int temp=0;
	
	/*�΂ߐ���2���T������*/
	y = binary_UnderMaxSearch(diagonal_nums,pN,target);
	
	if(y<0){
		//�Y�����Ȃ�
		return 0;
	}else if (target == diagonal_nums[y]){
		return target;	
	}
	
	x = y+1;
	maxValue=diagonal_nums[y];
	
	while(prices[x] < target && y>-1){
		temp =prices[x]+prices[y];
				
		if(target>temp){
			if(maxValue<temp){
				maxValue = temp;
			}
			x++;
		}else if(target<temp){
			y--;
		}else if(target == temp){
			return temp;
		}
	}
	return maxValue;	
}

int binary_UnderMaxSearch(int *diagonal_nums,int len_diagonal_nums,int target){
	//�ŏ��̓Y��
	int min=0;
	//�ő�̓Y��
	int max=len_diagonal_nums-1;
	//�����̗v�f�ԍ�
	int mid;

	if(target<diagonal_nums[min]){
		return -1;
	}

	//�ő�l�ƍŏ��l����v����܂Ń��[�v
	while(min<=max){

		mid=(min+max)/2;

		//��v�����炻�ꂪ����
		if(diagonal_nums[mid]==target){
			return mid;
		}else if(diagonal_nums[mid]<target){
			//�ړI�l�������l������Ȃ�A�ŏ��l�𒆉��l�̈��ɐݒ�
			min=mid+1;
		}else if(diagonal_nums[mid]>target){
			//�ړI�l�������l�������Ȃ�A�ő�l�𒆉��l�̈���ɐݒ�
			max=mid-1;
		}
	}
	while(diagonal_nums[mid] > target){
		mid--;
	}
	return mid;
}