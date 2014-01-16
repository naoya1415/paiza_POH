#include<stdio.h>
#include<stdlib.h>
#include<string.h>



#define inputSize  4002412	/*想定される標準入力の最大サイズ*/


int read_int( char *start, char **stop );		/*入力の読み取り高速化のため、標準入力のバッファから先頭の数字を変換して抜き出す*/								
void distsort(int n, const int a[], int b[]);	/*分布数えソート(ネットからコピペ)*/
int searchMax(int target,int *prices,			/*探索機能本体*/
			  int *diagonal_nums,int pN);
int binary_UnderMaxSearch(int *diagonal_nums,	/*diagonal_numsの中から、<=len_diagonal_numsの値を2分探索によって探索する	*/
						  int len_diagonal_nums,
						 int target);


int main(){						   
	
	int pN=0;				/*商品の総数*/
	int dN=0;				/*キャンペーンの日数*/						
	
	int *prices_before;		/*ソート前の商品価格の配列*/
	int *prices;			/*ソート済みの商品価格の配列*/
	int *target;			/*問題毎の目標価格の配列*/

	int *diagonal_nums;		/*探索アルゴリズムにて、価格表を斜めの情報を保存する配列*/
	
	char *raw_input;		/*入力の読み取り高速化のため、標準入力をfreadした結果を格納する配列*/				
	char *top;				/*入力の読み取り高速化のためのfp(標準入力の次のスキャン位置を格納)*/								
	int i,m,n;
	
	/*想定される最大入力サイズから、スタック領域での容量確保ができなかったため、
	ヒープ領域で確保する*/
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

	 //実比較部分
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

	


/*以下の変数はread_int専用のものだが、呼び出し回数が非常に多い(最大5万回)ため、
変数の宣言コストを節約を目的としてグローバル化した*/				

int STDR_word;		/*startから一文字ずつcharを取得し、intに変換した値を格納*/		

char *STDR_nums;	/*数字と確定した文字列への読み出しに利用するポインタ*/
int STDR_len=0;		/*数字と確定した文字列の長さ*/		
int STDR_readPoint;	/*数字と確定した文字列の内、最後尾から1桁ずつ下がっていくポインタ*/   
int STDR_result=0;	/*最終的に抽出される数字を格納する*/

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
	
	/*斜め線を2分探索する*/
	y = binary_UnderMaxSearch(diagonal_nums,pN,target);
	
	if(y<0){
		//該当しない
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
	//最小の添字
	int min=0;
	//最大の添字
	int max=len_diagonal_nums-1;
	//中央の要素番号
	int mid;

	if(target<diagonal_nums[min]){
		return -1;
	}

	//最大値と最小値が一致するまでループ
	while(min<=max){

		mid=(min+max)/2;

		//一致したらそれが答え
		if(diagonal_nums[mid]==target){
			return mid;
		}else if(diagonal_nums[mid]<target){
			//目的値が中央値よりも上なら、最小値を中央値の一つ上に設定
			min=mid+1;
		}else if(diagonal_nums[mid]>target){
			//目的値が中央値よりも下なら、最大値を中央値の一つ下に設定
			max=mid-1;
		}
	}
	while(diagonal_nums[mid] > target){
		mid--;
	}
	return mid;
}