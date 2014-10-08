/*------------------------------------------------------------------------
	* file:BiTree.c
	* date:1-10-2014
	* author:doodlesomething@63.com
	* version:1.0
	* description:二叉树的基本操作
	* attention:这里的二叉树中无法满足二叉树的节点用#代替以构成二叉树的形式，但是不会生成节点
--------------------------------------------------------------------------*/


#include <stdio.h>
#include "bitree.h"
#include "linkqueue.h"


/*
* @param BiTree *T
* @return Status
* @description:初始化二叉树
*/
Status InitBiTree(BiTree *T) {
	*T = NULL;

	return OK;
}

/*
* @param BiTree T
* @description:判空 空树则返回TRUE ，非空树返回FALSE
*/
Status BiTreeEmpty(BiTree T) {
	return T == NULL;
}


/*
* @description:二叉树的深度
* @param BiTree T
* @return int depth
*/
int BiTreeDepth(BiTree T) {
	int l,r;

	if(T) {
		l = BiTreeDepth(T->lchild);
		r = BiTreeDepth(T->rchild);
		return (l > r ? l : r) + 1; 
	}

	return 0;
}

/*
* @param BiTree T
* @return TElemType 
* @description:返回树的根的值
*/
TElemType Root(BiTree T) {
	return T->data;
}


/*
* @description:返回节点node的值
*/
TElemType Value(Position node) {
	return node->data;
}

/*
* @description:返回某个值在树中节点的指针，
		如果没有此值则返回NULL
* @more:使用队列实现
*/
Position NodePoint(BiTree T,TElemType elem) {
	if(T == NULL)
		return NULL;

	Position p;
	LinkQueue TreeQueue;

	InitQueue(&TreeQueue);
	p = T;

	EnQueue(&TreeQueue,p);

	while(!QueueEmpty(TreeQueue)) {
		DeQueue(&TreeQueue,&p);

		if(p->data == elem)
			return p;

		if(p->lchild)
			EnQueue(&TreeQueue,p->lchild);
		if(p->rchild)
			EnQueue(&TreeQueue,p->rchild);
	}
	
	return NULL;
}


/*
* @description:返回元素elem的左孩子的值
*/
TElemType LeftChild(BiTree T,TElemType elem) {
	Position p;

	p = NodePoint(T,elem);
	
	if(p->lchild)
		return p->lchild->data;
	
	return NULL;
}


/*
* @description:返回元素elem的右孩子的值
*/
TElemType RightChild(BiTree T,TElemType elem) {
	Position p;

	p = NodePoint(T,elem);
	
	if(p->rchild)
		return p->rchild->data;
	
	return NULL;
}

/*
* @description:返回节点值为elem的父亲节点的值
*/
TElemType Parent(BiTree T,TElemType elem) {
	Position p;
	LinkQueue TreeQueue;
	
	InitQueue(&TreeQueue);
	p = T;
	EnQueue(&TreeQueue,p);

	while(!QueueEmpty(TreeQueue)) {
		DeQueue(&TreeQueue,&p);

		if(p->lchild && p->lchild->data == elem || p->rchild && p->rchild->data == elem)
			return p->data;
		else {
			if(p->lchild)
				EnQueue(&TreeQueue,p->lchild);
			if(p->rchild)
				EnQueue(&TreeQueue,p->rchild);
		}
	}

	return NULL;
}

/*
* @description:返回节点值为elem的左兄弟
*/
TElemType LeftSibling(BiTree T,TElemType elem) {
	Position p;
	//父亲节点的指针
 	p = NodePoint(T,Parent(T,elem));

	if(p->lchild && p->rchild && p->rchild->data == elem)
		return p->lchild->data;

	return NULL;
}

/*
* @description:返回节点值为elem的右兄弟
*/
TElemType RightSibling(BiTree T,TElemType elem) {
	Position p;
	//父亲节点的指针
	p = NodePoint(T,Parent(T,elem));

	if(p->rchild && p->lchild && p->lchild == elem)
		return p->rchild->data;

	return NULL;
}



/*
* @description:将节点node的值赋值为value
*/
Status Assign(Position *node,TElemType value) { 
	(*node)->data = value;
	return OK;
}


/*
* @param BiTree *T
* @return Status
* @description:前序建立二叉树
*/
Status CreateBiTree(BiTree *T) {
	TElemType ch;
	
	scanf("%c",&ch);

	//下面的这两句如果不是将两颗不同的树的话可以删去
	while(ch == '\n')
		scanf("%c",&ch);

	if(ch == '#') 
		*T = NULL;
	else {
		*T = (BiTree) malloc(sizeof(struct BiTNode));
		if(!(*T))
			exit(OVERFLOW);

		(*T)->data = ch;
		//递归建立左右子树
		CreateBiTree(&(*T)->lchild);
		CreateBiTree(&(*T)->rchild);
	}

	return OK;
}


/*
* @description:前序遍历二叉树
*/
Status PreOrderTraverse(BiTree T,Status (*visit) (TElemType elem)) {
	if(T) {
		if(visit(T->data))
			if(PreOrderTraverse(T->lchild,visit))
				if(PreOrderTraverse(T->rchild,visit))
					return OK;
		return ERROR;
	}
	
	return OK;
}


/*
* @description:中序遍历二叉树
*/
Status InOrderTraverse(BiTree T,Status (*visit)(TElemType elem)) {
	if(T) {
		if(InOrderTraverse(T->lchild,visit))
			if(visit(T->data))
				if(InOrderTraverse(T->rchild,visit))
					return OK;
		return ERROR;
	}

	return OK;
}


/*
* @description:后序遍历二叉树
*/
Status PostOrderTraverse(BiTree T,Status (*visit)(TElemType elem)) {
	if(T) {
		if(PostOrderTraverse(T->lchild,visit))
			if(PostOrderTraverse(T->rchild,visit))
				if(visit(T->data))
					return OK;
		return ERROR;
	}

	return OK;
}


/*
* @description:层序遍历
* @more:利用队列，将每个节点的左右孩子入队，
	由于队列的性质，可按层遍历二叉树
*/
Status LevelOrderTraverse(BiTree T,Status (*visit) (TElemType elem)) {
	if(T == NULL)
		return ERROR;

	LinkQueue TreeQueue;
	ElemType p;

	InitQueue(&TreeQueue);
	p = T;

	EnQueue(&TreeQueue,p);

	while(!QueueEmpty(TreeQueue)) {
		DeQueue(&TreeQueue,&p);

		if(p) {
			visit(p->data);
			EnQueue(&TreeQueue,p->lchild);
			EnQueue(&TreeQueue,p->rchild);
		}
	}

	return OK;
}



/*
* @param TElemType elem
* @description:打印元素 -->遍历元素使用
*/
Status PrintElem(TElemType elem) {
	printf("%c",elem);
	return OK;
}



/*
* @param BiTree T
* @param int nLayer
* @return void
* @description:树状打印树
*/
void PrintAsTree(BiTree T,int nLayer) {
	int i;
	if(T == NULL)
		return;
	
	PrintAsTree(T->lchild,nLayer+1);
	for(i = 0; i < nLayer;i++)
		printf("  ");
	printf("%c\n",T->data);
	PrintAsTree(T->rchild,nLayer+1);
}


/*
* @description:销毁二叉树
*/
void DestoryBiTree(BiTree *T) {
	if(*T) {
		if((*T)->lchild)
			DestoryBiTree((*T)->lchild);
		if((*T)->rchild)
			DestoryBiTree((*T)->rchild);
		free(*T);
	}
}

/*
* @description:删除子树
		LR 等于0代表删除左子树
		LR == 1代表删除右子树
*/
Status DeleteChild(Position p,int LR) {
	if(LR == 0) {
		if(p->lchild) {
			DestoryBiTree(&p->lchild);
			return OK;
		}
	}
	else if(LR == 1) {
		if(p->rchild) {
			DestoryBiTree(&p->rchild);
			return OK;
		}
	}


	return ERROR;
	
}


/*
* @description:插入子树
		p指向T中某个节点,非空二叉树c和T不相交且右子树为空
		根据LR的值，插入c为T的p节点的左右子树，p节点的左或右子树成为c的右子树
*/
Status InsertChild(Position p,int LR,BiTree c) {
	if(p) {
		if(LR == 0) {
			c->rchild = p->lchild;
			p->lchild = c;
		}
		else if(LR == 1) {
			c->rchild = p->rchild;
			p->rchild = c;
		}

		return OK;
	}
	else
		return ERROR;
}



/*------------------------------------------------------------------------------------------------
* @description:分割线下面是二叉树的一些练习
*/


/*
* @param BiTree T
* @return void
* @description:递归算法将二叉树的左右子树交换
*/
void Exchange(BiTree T) {
	BiTree temp;

	if(T) {
		//交换必须从最深层开始
		Exchange(T->lchild);
		Exchange(T->rchild);
		//交换左右孩子节点的指针
		temp = T->lchild;
		T->lchild = T->rchild;
		T->rchild = temp;
	}
}




/*
* @param BiTree T
* @description:统计二叉树中叶子节点的数目
* @more:所谓叶子节点即即没有左子树和右子树的节点（度为0）
*/
int LevesNum(BiTree T) {
	if(T) {
		//发现叶子节点
		if(T->lchild == NULL && T->rchild == NULL)
			return 1;

		//递归实现查找左右子树
		return LevesNum(T->lchild) + LevesNum(T->rchild);
	}

	return 0;
}




/*
* @param BiTree T1
* @param BiTree T2
* @return Status
* @description:判断两个二叉树是否相等
* @more:主要树为空的情况
*/
Status EqualBiTree(BiTree T1,BiTree T2) {
	int flag;
	//请初始化，否则当为假时，返回值未知
	flag = 0;

	if(NULL == T1 && NULL == T2)
		flag = TRUE;
	else {
		if(T1 != NULL && T2 != NULL) {
			//当前节点相等
			if(T1->data == T2->data)
				//左孩子相等
				if(EqualBiTree(T1->lchild,T2->lchild))
					//真假情况决定于右孩子
					flag = EqualBiTree(T1->rchild,T2->rchild);
		}
	}
	
	return flag;
}


/*
* @param BiTree T
* @return int nodenum
* @description:递归求二叉树的节点数
*/
int NodeNum(BiTree T) {
	if(NULL == T)
		return 0;
	//证明树不空则至少有一个根节点
	else
		return NodeNum(T->lchild) + NodeNum(T->rchild) + 1;

}

/*
* @param BiTree T
* @param TElemType elem;
* @description:判断一个元素值是否在二叉树中
* @more:在则返回真，否则返回假-->可知在比较过程中(比较两颗树是否等)
  可先比较当前节点，再比较左子树，最后比较右子树
*/
Status Locate(BiTree T,TElemType elem) {
	if(NULL == T)
		return ERROR;
	else {
		if(T->data == elem)
			return TRUE;
		else {
			if(! Locate(T->lchild,elem))
				return Locate(T->rchild,elem);
			else
				return TRUE;
		}
	}
}
