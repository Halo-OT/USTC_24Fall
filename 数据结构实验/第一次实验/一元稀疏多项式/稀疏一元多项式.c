#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define N 7

//定义头指针
typedef struct Pitem{
    float xishu;
    float exp;
    struct Pitem *next;
} Pitem;

//定义每一项内容
typedef struct Phead{
    int number ;
    Pitem *next;
} Phead;

//建立头指针
Phead* creatPhead(){
    Phead *head=( Phead* )malloc(sizeof(Phead));
    head->next=NULL;
    head->number = 0;
    return head;
}

//建立各项
Pitem * creatPitem(float xishu,float exp){
    Pitem * newitem = ( Pitem* )malloc( sizeof(Pitem) );
    newitem->xishu=xishu;
    newitem->exp=exp;
    newitem->next=NULL;
    return newitem;
}

//遍历求链表项数
int Pnumber(Phead* head){
    int number = 0;
    Pitem* current = head->next;
    while (current != NULL){
        number += 1;
        current = current->next;
    }
    return number;   
}

//插入各项
void Pinsert(Phead * head,float xishu,float exp){
    Pitem *newitem = creatPitem(xishu, exp);
    
    if (xishu == 0){
        free ( newitem );
        return;
    }
    // 插入到头节点的前面（即如果链表为空，或者新节点的指数大于第一个节点的指数）
    if (head->next == NULL || head->next->exp < exp) {
        newitem->next = head->next;
        head->next = newitem;
        return;
    }

    Pitem *current = head;  // 从头节点开始遍历

    // 循环遍历，找到合适的插入点
    while (current->next != NULL && current->next->exp > exp) {
        current = current->next;
    }

    // 如果找到相同指数的项，合并系数
    if (current->next != NULL && current->next->exp == exp) {
        current->next->xishu += xishu;
        free(newitem);

        // 如果系数为 0，删除该项
        if (current->next->xishu == 0) {
            Pitem *temp = current->next;
            current->next = temp->next;
            free(temp);
        }
    } 
    // 如果没有相同指数的项，将新项插入合适位置
    else {
        newitem->next = current->next;
        current->next = newitem;
    }

}

//找到目标项目并删除 
void Pdelete(Phead* head,float target){
    int flag = 0;
    if ( head->next==NULL || head->next->exp < target ){
        flag = 0;
   } else {
        Pitem * current = head;
        //循环判断 
        while( current->next != NULL ){
            Pitem* backward =current->next;
            if ( current->next->exp == target ){
                flag = 1;
                current->next = backward->next;
                free(backward);
                break;
            } else current = current->next; 
        }
    }
   
    if(flag) {
        printf("已经找到目标项目喵～\n");
        printf("修改成功喵～\n");
    } 
    else{
        printf("未能找到目标项目喵～\n");
        printf("修改失败喵～\n");
    } 
    

    head->number = Pnumber( head );
}

//修改目标项目指数、系数
void Pmodify(Phead *head, float old_exp, float new_xishu, float new_exp) {
    // 先删除原节点，再插入新节点（防止指数冲突）
    Pdelete(head, old_exp);
    Pinsert(head, new_xishu, new_exp);
}

//fixP菜单 Menu
void fixMenu(){
    printf("\n请选择操作类型:\n");
    printf("1. 插入新的节点\n");
    printf("2. 删除已有的节点\n");
    printf("3. 修改已有节点的系数和指数\n");
    printf("4. 退出操作\n");
    printf("输入您的选择: ");
}

//修改链表 插入、删除、修改节点的系数&指数 
void fixP(Phead *head){
    int choice;
    float xishu;
    float target;
    float exp,new_exp;

    fixMenu();
    scanf("%d", &choice );

    switch (choice) {
            case 1:
                // 插入新的节点

                printf("请输入要插入的系数喵～: ");
                scanf(" %f", &xishu);
                printf("请输入要插入的指数喵～: ");
                scanf(" %f", &exp);
                Pinsert(head, xishu, exp);
                printf("插入成功！\n");
                break;

            case 2:
                // 删除已有的节点
                printf("请输入要删除的节点的指数喵～: ");
                scanf(" %f", &exp);
                Pdelete(head, exp);
                break;

            case 3:
                printf("请输入要修改的节点的原指数喵～: ");
                scanf("%f", &exp);

                printf("请输入新的系数喵～: ");
                scanf("%f", &xishu);
                printf("请输入新的指数喵～: ");
                scanf("%f", &new_exp);
                Pmodify(head, exp, xishu, new_exp);

                break;
            
            default:
                printf("你好像输入错误了，请重新选择喵～。\n");
                break;
    }

}

//多项式建立函数（插入法建立）
void creatP(Phead* head ){
    float xishu;
    float exp;
    char choice;

    printf("输入多项式的每一项 (系数和指数)，输入 'n' 停止:\n");
    do {
        printf("输入系数: ");
        scanf("%f", &xishu);
        printf("输入指数: ");
        scanf("%f", &exp);
        //插入法建立链表
        Pinsert(head,xishu,exp);
        printf("继续输入下一项吗？(y/n): ");
        scanf(" %c", &choice);  // 注意前面的空格，以便正确读取字符
    } while (choice == 'y' || choice == 'Y');
    head->number = Pnumber( head );
}

//多项式输出
void displayP(Phead *head){
    Pitem * current = head->next;
    if (!current) {
        printf("多项式为空喵～\n");
    } 
    
    else{
        printf("          ");
        while(current->next != NULL ){
            if(current->exp == 0){
                printf(" %.3f +",current->xishu);
            } 

            //针对不定积分ln｜x｜的输出
            else if (current->exp == -1.5 ){

                if (current->xishu==1){
                printf(" ln|x| +");
            } else if (current->xishu== -1){
                printf(" (-ln|x|) +");
            } else printf(" %.3fln|x| +",current->xishu);

            //针对系数为正负1的输出
            } else{
                if (current->xishu==1){
                printf(" x^%.0f +",current->exp);
            } else if (current->xishu== -1){
                printf(" (-x^%.0f) +",current->exp);
            } else printf(" %.3fx^%.0f +",current->xishu,current->exp);
       
            }
        
            current = current->next;
        }
        //末尾项输出 末尾无’+‘
        if(current->exp == 0){
            printf(" %.3f\n ",current->xishu);
        } 
        else{
            if (current->xishu==1){
            printf(" x^%.0f\n ",current->exp);
        } else if (current->xishu== -1){
            printf(" (-x^%.0f)\n ",current->exp);
        } else printf(" %.3fx^%.0f\n ",current->xishu,current->exp);
       
        }
        printf("          多项式共有%d项\n",head->number);
    }
    
}

//多项式求值函数
float Pvalue(Phead *head, float x) {
    float result = 0;
    Pitem* current = head->next;
    while (current != NULL) {
        if (current->exp != -1.5) {
            result += current->xishu * (pow(x, current->exp));
        } else {
            if (x <= 0) {
                printf("Error: Logarithm undefined for non-positive values.\n");
                return NAN;  // 或者其他适当的错误处理
            }
            result += current->xishu * log(abs(x));
        }
        current = current->next;
    }
    return result;
}

//多项式清空+free空间
void clearP(Phead* head){
    Pitem* current = head->next;
    Pitem* nextitem;
    while(current != NULL){
        nextitem = current->next;
        free(current);
        current = nextitem;
    }
}

//多项式加法函数
/*
Phead* addP(Phead* head1,Phead* head2){
    Phead* result = creatPhead();
    Pitem* p1 = head1->next;
    Pitem* p2 = head2->next;
    while( p1  != NULL && p2 != NULL ){
        if(p1->exp > p2->exp){
            Pinsert(result, p1->xishu, p1->exp);
            p1=p1->next;
        } 
        else if (p2->exp > p1->exp){
            Pinsert(result, p2->xishu, p2->exp);
            p2=p2->next;
        }
        else{
            Pinsert(result, p1->xishu, p1->exp);
            Pinsert(result, p2->xishu, p2->exp);
            p1=p1->next;
            p2=p2->next;
        }
    }
    result->number = Pnumber( result );
    return result;
}
*/
Phead* addP(Phead* head1, Phead* head2) {
    Phead* result = creatPhead();
    Pitem* p1 = head1->next;
    Pitem* p2 = head2->next;

    while (p1 != NULL ) {
        Pinsert(result, p1->xishu, p1->exp); // 直接插入p1项
        p1 = p1->next;
    }
    while (p2 != NULL) {
        Pinsert(result, p2->xishu, p2->exp); // 直接插入p2项
        p2 = p2->next;
    }
    
    

    /*
    while (p1 != NULL || p2 != NULL) {
        if (p1 != NULL && (p2 == NULL || p1->exp > p2->exp)) {
            Pinsert(result, p1->xishu, p1->exp); // 直接插入p1项
            p1 = p1->next;
        } 
        else if (p2 != NULL && (p1 == NULL || p2->exp > p1->exp)) {
            Pinsert(result, p2->xishu, p2->exp); // 直接插入p2项
            p2 = p2->next;
        } 
        else { // 当p1和p2的指数相等
            float new_xishu = p1->xishu + p2->xishu; // 合并系数
            if (new_xishu != 0) { // 只有非零系数的项才插入
                Pinsert(result, new_xishu, p1->exp);
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
    */

    result->number = Pnumber(result); // 更新项的数量
    return result;
}


//多项式减法函数
/*
Phead* minusP(Phead* head1,Phead* head2){
    Pitem* p = head2->next;
    //遍历使head2系数取相反数   
    while(p != NULL){
        p->xishu = -p->xishu;
        p = p->next;
    }

    Phead* result=addP(head1,head2);
    result->number = Pnumber( result );
    return result;
}
*/
Phead* minusP(Phead* head1, Phead* head2) {
    Phead* negated_head2 = creatPhead();
    Pitem* p = head2->next;

    // 遍历head2，并将每项系数取反，添加到新多项式negated_head2中
    while (p != NULL) {
        Pinsert(negated_head2, -p->xishu, p->exp);
        p = p->next;
    }

    Phead* result = addP(head1, negated_head2); // 使用negated_head2进行加法
    result->number = Pnumber(result);
    
    // 清理临时的negated_head2多项式
    clearP(negated_head2);
    free(negated_head2);
    
    return result;
}

// 多项式微分并覆盖原来的多项式
void Pderivative(Phead* head) {
    Pitem *current = head->next;  // head 是链表头，跳过头节点，处理实际的多项式项
    Pitem *prev = head;  // 用于跟踪前一个节点

    while (current != NULL) {
        if (current->exp != 0) {
            // 非常数项
            current->xishu *= current->exp;
            current->exp--;
            prev = current;  // 更新前一个节点
        } else {
            // 如果是常数项，删除它
            prev->next = current->next;  // 跳过当前节点
            free(current);  // 释放当前节点
        }
        current = prev->next;  // 继续处理下一个节点
    }
    // 更新链表项的数量
    head->number = Pnumber(head);
}

//多项式不定积分
void Pintegral(Phead *head) {
    Pitem *current = head->next;  // 获取第一个多项式项
    Pitem *prev = head;  // 用于跟踪前一个节点

    // 遍历每一项进行积分
    while (current != NULL) {
        if (current->exp == -1) {
            // 处理 x^-1 的项，积分结果为 ln|x|，可以存储为一个特定标志（例如指数为 -1.5 代表 ln|x| 项）
            current->xishu = current->xishu;  // 系数不变
            current->exp = -1.5;  // 用 -1.5 来表示 ln|x| 项，区分它和普通 x^n 项
        } else {
            // 正常处理其他项
            current->exp++;
            current->xishu = current->xishu / current->exp;
        }

        prev = current;
        current = current->next;
    }

    // 添加积分常数项 C（默认值设为 0）
    float C = 0.0;
    Pitem *constant_term = creatPitem(C, 0);  // C 项的指数为 0
    prev->next = constant_term;

}

//多项式定积分
float Pdefinite_integrals(Phead* head, float x1, float x2){
    float result = 0;
    Pintegral( head );
    result = Pvalue( head , x1 ) - Pvalue( head, x2 );
    return result;
}

// 菜单显示函数
void displayMenu() {
    printf("===================================\n");
    printf("            多项式计算器            \n");
    printf("===================================\n");
    printf("1. 输入多项式\n");
    printf("2. 显示多项式\n");
    printf("3. 多项式加法\n");
    printf("4. 多项式减法\n");
    printf("5. 多项式求值\n");
    printf("6. 清空多项式\n");
    printf("7. 销毁多项式\n");
    printf("8. 修改多项式某节点\n");
    printf("9. 多项式求微分\n");
    printf("10. 多项式求不定积分\n");
    printf("11. 多项式定积分\n");
    printf("0. 退出\n");
    printf("===================================\n");
    printf("请输入您的选择喵～: ");
}

int main() {
    
    int choice, a;
    float x;
    int c;//选择的多项式
    static int num =0;//静态变量num 用于记录多项式数目
    int n;//记录要求几阶导数
    Phead* P[N] = { NULL };

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
    
            //输入多项式
            case 1: {
                printf("您选择了输入多项式喵～\n");
                
                char choice;
                do{
                    if (num >= N) {  // N 是最大多项式数量，防止数组越界
                        printf("已达到最大多项式数量喵～\n");
                        break;
                    }
                    P[num]=creatPhead();
                    creatP( P[num] );
                    printf("继续输入下一个多项式吗？(y/n): ");
                    scanf(" %c", &choice);  // 注意前面的空格，以便正确读取字符
                    num++;
                } while (choice == 'y' || choice == 'Y');
                
                break;
            }

            //显示多项式
            case 2: {
                printf("您选择了显示多项式喵～\n");
                /* headsort( P, N ); */
                for (int j = 0; j < N; j++) {
                    if (P[j] != NULL) {  // 确保该多项式已初始化
                        printf("多项式P%d为喵～:\n", j + 1);
                        displayP(P[j]);
                    } else {
                        printf("多项式P%d为空喵～\n", j + 1);
                    }
                }
                printf("多项式输出完成喵～\n");
                
                break;
            }

            //多项式加法
            case 3: {
                printf("您选择了多项式加法喵～\n");
                printf("请输入两个个多项式P1、P2喵～：\n");
                printf("如果您想使用之前已经使用过的多项式，请输入 1 喵，否则请输入 0 喵~: ");
                
                scanf("%d",&a);

                switch (a) {
                    case 0:{
                        Phead* head1 = creatPhead();
                        creatP(head1);

                        Phead* head2 = creatPhead();
                        creatP(head2);

                        Phead* result = addP(head1,head2);

                        //存储result到P的最后一个
                        num+=1;
                        P[num-1]=result;
                        displayP(result);

                        //释放head1&2的空间
                        clearP(head1);
                        clearP(head2);
                        free(result);
                        break;
                    }

                    case 1:{
                        int n1,n2;
                        printf("请选择要相加的两条多项式序号(中间用空格隔开): ");

                        scanf(" %d %d",&n1 ,&n2);
                        Phead* result=addP(P[n1-1], P[n2-1]);

                        P[n1-1]=result;
                        displayP(result);
                        
                        free(result);
                        break;
                    }
                    default:
                        printf("您好像输入了不正确的项喵～\n请重新输入喵～");
                        break;
                }    

                break;
            }   
            
            //多项式减法    
            case 4: {
                printf("您选择了多项式减法喵～\n");
                printf("请输入两个个多项式P1、P2 (P1-P2) 喵～：\n");
                printf("如果您想使用之前已经使用过的多项式，请输入 1 喵( 先前输入的将被视为被减数 ），否则请输入 0 喵~: ");
                
                scanf("%d",&a);

                switch (a) {
                    case 0:{
                        Phead* head1 = creatPhead();
                        creatP(head1);

                        Phead* head2 = creatPhead();
                        creatP(head2);

                        Phead* result = minusP(head1,head2);

                        num+=1;
                        P[num-1]=result;
                        displayP(result);

                        //清除输入的head1、2 和 产生的result
                        clearP( head1 );
                        clearP( head2 );
                        free(result);
                        break;
                    }

                    case 1:{
                        int n1,n2;
                        printf("请选择要相减的两条多项式序号(中间用空格隔开): ");

                        scanf(" %d %d",&n1 ,&n2);

                        Phead* result=minusP(P[n1-1], P[n2-1]);

                        P[n1-1]=result;
                        displayP(result);
                        //清除输入的head1、2 和 产生的result
                        free(result);
                        break;
                    }
                    default:
                        printf("您好像输入了不正确的项喵～\n请重新输入喵～");
                        break;
                }
                break;
            }
            
            //多项式求值
            case 5: {
                printf("您选择了多项式求值喵～\n");
                printf("请您选择要求值的多项式序号喵～: ");
                scanf(" %d", &c);
                printf("请输入x的值喵～: ");
                scanf(" %f", &x);

                float result=Pvalue(P[c-1],x);
                printf("代入后求得的多项式值为 %f 喵～",result);

                break;
            }

            //多项式清空
            case 6: { 
                printf("您选择了清空多项式喵～\n");
                printf("请您选择要清空的多项式序号喵～: ");
                scanf("%d", &c);
                clearP( P[c-1] );
                num--;
                printf("多项式已清空喵～\n");
                break;
            }

            //多项式销毁
            case 7: {
                printf("您选择了销毁多项式喵～\n");
                printf("请您选择要销毁的多项式序号喵～: ");
                scanf("%d", &c);
                clearP( P[c-1] );
                P[c-1]=NULL;
                num--;
                printf("多项式已销毁喵～\n");
                break;
            }
            
            //修改多项式中项目
            case 8: {
                
                printf("请您选择要修改的多项式序号喵～: ");
                scanf(" %d", &c);
                if(P[c-1] == NULL ){
                    printf("该多项式为空喵～ 无法修改喵～\n");
                } else{
                    fixP(P[c-1]);
                    P[c-1]->number = Pnumber(P[c-1]);
                }
                
                break;
            } 
                
            //多项式求微分
            case 9:{ 
                printf("请选择您要求微分的多项式序号喵～: ");
                scanf(" %d",&c);

                // 检查用户选择的多项式是否为空
                if (P[c-1] == NULL || P[c-1]->number == 0) {
                    printf("该多项式为空喵～ 无法进行微分喵～\n");
                    break;  
                }

                printf("请输入您要求几阶导数: ");
                scanf(" %d",&n);
                
                for (int i = 0; i < n; i++){
                    if(P[c-1]->number == 0 ){
                        printf("该多项式在第%d次求微分前时已为 0 喵～ 无法继续求微分d喵～\n",i+1);
                        clearP(P[c-1]);
                        break;

                    } else {
                        Pderivative(P[c-1]);
                        // 确保多项式不为空才显示
                        // 检查微分后是否为0
                        if (P[c - 1]->number == 0) {
                            printf("多项式在第%d次求微分后变为0喵～\n", i + 1);
                            break;  
                        }
                    }
                }
                // 输出微分后的结果
                if (P[c - 1]->number > 0) {  
                    printf("微分结果为：\n");
                    displayP(P[c - 1]);
                       
                }
                break; 
            }
            
            //多项式求不定积分
            case 10:{
                printf("请选择您要积分的多项式序号喵～: ");
                scanf(" %d",&c);

                // 检查用户选择的多项式是否为空
                if (P[c-1] == NULL || P[c-1]->number == 0) {
                    printf("该多项式为空喵～ 无法进行积分喵～\n");
                    break;  
                }

                Pintegral(P[c-1]);
                // 输出积分后的结果
                printf("积分结果为：\n");
                displayP(P[c-1]);
                break;
                }

            //多项式求定积分
            case 11:{
                float x1, x2;
                printf("请选择您要积分的多项式序号喵～: ");
                scanf(" %d",&c);
                 // 检查用户选择的多项式是否为空
                if (P[c-1] == NULL || P[c-1]->number == 0) {
                    printf("该多项式为空喵～ 无法进行积分喵～\n");
                    break;  
                }
                printf("请输入积分的上限喵～: ");
                scanf(" %f",&x1);
                printf("请输入积分的下限喵～: ");
                scanf(" %f",&x2);
                float result = Pdefinite_integrals(P[c-1], x1, x2);
                printf("定积分求得的值为 %f 喵～\n",result );

            }   
            break;

            //退出程序
            case 0: {
                printf("程序退出。感谢使用喵～\n");
                for(int i=0;i<N ;i++){
                    clearP( P[i] );
                    P[i]=NULL;
                }
               
                return 0;
            }

            default:
                printf("你好像输入错误了，请重新选择喵～。\n");
                break;
        }
        printf("\n");  // 添加一个空行用于美化输出
    }
    return 0;
}