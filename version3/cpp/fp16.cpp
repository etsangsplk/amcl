/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

/* AMCL Fp^8 functions */

/* FP16 elements are of the form a+ib, where i is sqrt(sqrt(-1+sqrt(-1))) */

#include "fp16_YYY.h"

using namespace XXX;

/* test x==0 ? */
int YYY::FP16_iszilch(FP16 *x)
{
    if (FP8_iszilch(&(x->a)) && FP8_iszilch(&(x->b))) return 1;
    return 0;
}

/* test x==1 ? */
int YYY::FP16_isunity(FP16 *x)
{
    if (FP8_isunity(&(x->a)) && FP8_iszilch(&(x->b))) return 1;
    return 0;
}

/* test is w real? That is in a+ib test b is zero */
int YYY::FP16_isreal(FP16 *w)
{
    return FP8_iszilch(&(w->b));
}

/* return 1 if x==y, else 0 */
int YYY::FP16_equals(FP16 *x,FP16 *y)
{
    if (FP8_equals(&(x->a),&(y->a)) && FP8_equals(&(x->b),&(y->b)))
        return 1;
    return 0;
}

/* set FP16 from two FP8s */
void YYY::FP16_from_FP8s(FP16 *w,FP8 * x,FP8* y)
{
    FP8_copy(&(w->a), x);
    FP8_copy(&(w->b), y);
}

/* set FP16 from FP8 */
void YYY::FP16_from_FP8(FP16 *w,FP8 *x)
{
    FP8_copy(&(w->a), x);
    FP8_zero(&(w->b));
}

/* set high part of FP16 from FP8 */
void YYY::FP16_from_FP8H(FP16 *w,FP8 *x)
{
    FP8_copy(&(w->b), x);
    FP8_zero(&(w->a));
}

/* FP16 copy w=x */
void YYY::FP16_copy(FP16 *w,FP16 *x)
{
    if (w==x) return;
    FP8_copy(&(w->a), &(x->a));
    FP8_copy(&(w->b), &(x->b));
}

/* FP16 w=0 */
void YYY::FP16_zero(FP16 *w)
{
    FP8_zero(&(w->a));
    FP8_zero(&(w->b));
}

/* FP16 w=1 */
void YYY::FP16_one(FP16 *w)
{
    FP8_one(&(w->a));
    FP8_zero(&(w->b));
}

/* Set w=-x */
void YYY::FP16_neg(FP16 *w,FP16 *x)
{
    /* Just one field neg */
    FP8 m,t;
    FP8_add(&m,&(x->a),&(x->b));
	FP8_norm(&m);
    FP8_neg(&m,&m);
    FP8_add(&t,&m,&(x->b));
    FP8_add(&(w->b),&m,&(x->a));
    FP8_copy(&(w->a),&t);
	FP16_norm(w);
}

/* Set w=conj(x) */
void YYY::FP16_conj(FP16 *w,FP16 *x)
{
    FP8_copy(&(w->a), &(x->a));
    FP8_neg(&(w->b), &(x->b));
	FP16_norm(w);
}

/* Set w=-conj(x) */
void YYY::FP16_nconj(FP16 *w,FP16 *x)
{
    FP8_copy(&(w->b),&(x->b));
    FP8_neg(&(w->a), &(x->a));
	FP16_norm(w);
}

/* Set w=x+y */
void YYY::FP16_add(FP16 *w,FP16 *x,FP16 *y)
{
    FP8_add(&(w->a), &(x->a), &(y->a));
    FP8_add(&(w->b), &(x->b), &(y->b));
}

/* Set w=x-y */
/* Input y MUST be normed */
void YYY::FP16_sub(FP16 *w,FP16 *x,FP16 *y)
{
    FP16 my;

    FP16_neg(&my, y);
    FP16_add(w, x, &my);

}

/* reduce all components of w mod Modulus */
void YYY::FP16_reduce(FP16 *w)
{
    FP8_reduce(&(w->a));
    FP8_reduce(&(w->b));
}

/* normalise all elements of w */
void YYY::FP16_norm(FP16 *w)
{
    FP8_norm(&(w->a));
    FP8_norm(&(w->b));
}

/* Set w=s*x, where s is FP8 */
void YYY::FP16_pmul(FP16 *w,FP16 *x,FP8 *s)
{
    FP8_mul(&(w->a),&(x->a),s);
    FP8_mul(&(w->b),&(x->b),s);
}

/* Set w=s*x, where s is FP2 */
void YYY::FP16_qmul(FP16 *w,FP16 *x,FP2 *s)
{
    FP8_qmul(&(w->a),&(x->a),s);
    FP8_qmul(&(w->b),&(x->b),s);
}

/* Set w=s*x, where s is int */
void YYY::FP16_imul(FP16 *w,FP16 *x,int s)
{
    FP8_imul(&(w->a),&(x->a),s);
    FP8_imul(&(w->b),&(x->b),s);
}

/* Set w=x^2 */
/* Input MUST be normed  */
void YYY::FP16_sqr(FP16 *w,FP16 *x)
{
    FP8 t1,t2,t3;

    FP8_mul(&t3,&(x->a),&(x->b)); /* norms x */
    FP8_copy(&t2,&(x->b));
    FP8_add(&t1,&(x->a),&(x->b));
    FP8_times_i(&t2);

    FP8_add(&t2,&(x->a),&t2);

	FP8_norm(&t1);  // 2
	FP8_norm(&t2);  // 2

    FP8_mul(&(w->a),&t1,&t2);

    FP8_copy(&t2,&t3);
    FP8_times_i(&t2);

    FP8_add(&t2,&t2,&t3);

	FP8_norm(&t2);  // 2
    FP8_neg(&t2,&t2);
    FP8_add(&(w->a),&(w->a),&t2);  /* a=(a+b)(a+i^2.b)-i^2.ab-ab = a*a+ib*ib */
    FP8_add(&(w->b),&t3,&t3);  /* b=2ab */

    FP16_norm(w);
}

/* Set w=x*y */
/* Inputs MUST be normed  */
void YYY::FP16_mul(FP16 *w,FP16 *x,FP16 *y)
{

    FP8 t1,t2,t3,t4;
    FP8_mul(&t1,&(x->a),&(y->a)); 
    FP8_mul(&t2,&(x->b),&(y->b)); 

    FP8_add(&t3,&(y->b),&(y->a));
    FP8_add(&t4,&(x->b),&(x->a));

	FP8_norm(&t4); // 2
	FP8_norm(&t3); // 2

    FP8_mul(&t4,&t4,&t3); /* (xa+xb)(ya+yb) */

	FP8_neg(&t3,&t1);  // 1
	FP8_add(&t4,&t4,&t3);  //t4E=3
    FP8_norm(&t4);

	FP8_neg(&t3,&t2);  // 1
	FP8_add(&(w->b),&t4,&t3); //wbE=3

    FP8_times_i(&t2);
    FP8_add(&(w->a),&t2,&t1);

    FP16_norm(w);
}

/* output FP16 in format [a,b] */
void YYY::FP16_output(FP16 *w)
{
    printf("[");
    FP8_output(&(w->a));
    printf(",");
    FP8_output(&(w->b));
    printf("]");
}

void YYY::FP16_rawoutput(FP16 *w)
{
    printf("[");
    FP8_rawoutput(&(w->a));
    printf(",");
    FP8_rawoutput(&(w->b));
    printf("]");
}

/* Set w=1/x */
void YYY::FP16_inv(FP16 *w,FP16 *x)
{
    FP8 t1,t2;
    FP8_sqr(&t1,&(x->a));
    FP8_sqr(&t2,&(x->b));
    FP8_times_i(&t2);
	FP8_norm(&t2);

    FP8_sub(&t1,&t1,&t2);
	FP8_norm(&t1);

    FP8_inv(&t1,&t1);

    FP8_mul(&(w->a),&t1,&(x->a));
    FP8_neg(&t1,&t1);
	FP8_norm(&t1);
    FP8_mul(&(w->b),&t1,&(x->b));
}

/* w*=i where i = sqrt(sqrt(-1+sqrt(-1))) */
void YYY::FP16_times_i(FP16 *w)
{
	FP8 s,t;
	FP8_copy(&s,&(w->b));
	FP8_copy(&t,&(w->a));
	FP8_times_i(&s);
	FP8_copy(&(w->a),&s);
	FP8_copy(&(w->b),&t);
	FP16_norm(w);
}

void YYY::FP16_times_i2(FP16 *w)
{
	FP8_times_i(&(w->a));
	FP8_times_i(&(w->b));
}

void YYY::FP16_times_i4(FP16 *w)
{
	FP8_times_i2(&(w->a));
	FP8_times_i2(&(w->b));
}

/* Set w=w^p using Frobenius */
void YYY::FP16_frob(FP16 *w,FP2 *f)
{ // f=(i+1)^(p-3)/8
	FP2 ff;
	//FP4 f4;
	FP2_sqr(&ff,f);  // (i+1)^(p-3)/4
	FP2_norm(&ff);

	FP8_frob(&(w->a),&ff);
	FP8_frob(&(w->b),&ff);

	//FP4_from_FP2(&f4,f); // (1+i)^(p-3)/8

	FP8_qmul(&(w->b),&(w->b),f);  // times (1+i)^(p-3)/8
	FP8_times_i(&(w->b));		// (i+1)^(p-1)/8
}

/* Set r=a^b mod m */
void YYY::FP16_pow(FP16 *r,FP16* a,BIG b)
{
    FP16 w;
    BIG z,zilch;
    int bt;

    BIG_zero(zilch);
    BIG_norm(b);
    BIG_copy(z,b);
    FP16_copy(&w,a);
    FP16_one(r);

    while(1)
    {
        bt=BIG_parity(z);
        BIG_shr(z,1);
        if (bt) FP16_mul(r,r,&w);
        if (BIG_comp(z,zilch)==0) break;
        FP16_sqr(&w,&w);
    }
    FP16_reduce(r);
}

#if CURVE_SECURITY_ZZZ == 256

/* XTR xtr_a function */
void YYY::FP16_xtr_A(FP16 *r,FP16 *w,FP16 *x,FP16 *y,FP16 *z)
{
    FP16 t1,t2;

    FP16_copy(r,x);
    FP16_sub(&t1,w,y);
	FP16_norm(&t1);
    FP16_pmul(&t1,&t1,&(r->a));
    FP16_add(&t2,w,y);
	FP16_norm(&t2);
    FP16_pmul(&t2,&t2,&(r->b));
    FP16_times_i(&t2);

    FP16_add(r,&t1,&t2);
    FP16_add(r,r,z);

    FP16_reduce(r);
}

/* XTR xtr_d function */
void YYY::FP16_xtr_D(FP16 *r,FP16 *x)
{
    FP16 w;
    FP16_copy(r,x);
    FP16_conj(&w,r);
    FP16_add(&w,&w,&w);
    FP16_sqr(r,r);
	FP16_norm(&w);
    FP16_sub(r,r,&w);
    FP16_reduce(r);    /* reduce here as multiple calls trigger automatic reductions */
}

/* r=x^n using XTR method on traces of FP12s */
void YYY::FP16_xtr_pow(FP16 *r,FP16 *x,BIG n)
{
    int i,par,nb;
    BIG v;
    FP2 w2;
	FP4 w4;
	FP8 w8;
    FP16 t,a,b,c;

    BIG_zero(v);
    BIG_inc(v,3);
	BIG_norm(v);
    FP2_from_BIG(&w2,v);
    FP4_from_FP2(&w4,&w2);
	FP8_from_FP4(&w8,&w4);
    FP16_from_FP8(&a,&w8);

	FP16_copy(&b,x);
    FP16_xtr_D(&c,x);

    BIG_norm(n);
    par=BIG_parity(n);
    BIG_copy(v,n);
    BIG_shr(v,1);
    if (par==0)
    {
        BIG_dec(v,1);
        BIG_norm(v);
    }

    nb=BIG_nbits(v);
    for (i=nb-1; i>=0; i--)
    {
        if (!BIG_bit(v,i))
        {
            FP16_copy(&t,&b);
            FP16_conj(x,x);
            FP16_conj(&c,&c);
            FP16_xtr_A(&b,&a,&b,x,&c);
            FP16_conj(x,x);
            FP16_xtr_D(&c,&t);
            FP16_xtr_D(&a,&a);
        }
        else
        {
            FP16_conj(&t,&a);
            FP16_xtr_D(&a,&b);
            FP16_xtr_A(&b,&c,&b,x,&t);
            FP16_xtr_D(&c,&c);
        }
    }

    if (par==0) FP16_copy(r,&c);
    else FP16_copy(r,&b);
    FP16_reduce(r);
}

/* r=ck^a.cl^n using XTR double exponentiation method on traces of FP12s. See Stam thesis. */
void YYY::FP16_xtr_pow2(FP16 *r,FP16 *ck,FP16 *cl,FP16 *ckml,FP16 *ckm2l,BIG a,BIG b)
{
    int i,f2;
    BIG d,e,w;
    FP16 t,cu,cv,cumv,cum2v;

    BIG_norm(a);
	BIG_norm(b);
    BIG_copy(e,a);
    BIG_copy(d,b);
    FP16_copy(&cu,ck);
    FP16_copy(&cv,cl);
    FP16_copy(&cumv,ckml);
    FP16_copy(&cum2v,ckm2l);

    f2=0;
    while (BIG_parity(d)==0 && BIG_parity(e)==0)
    {
        BIG_shr(d,1);
        BIG_shr(e,1);
        f2++;
    }
    while (BIG_comp(d,e)!=0)
    {
        if (BIG_comp(d,e)>0)
        {
            BIG_imul(w,e,4);
            BIG_norm(w);
            if (BIG_comp(d,w)<=0)
            {
                BIG_copy(w,d);
                BIG_copy(d,e);
                BIG_sub(e,w,e);
                BIG_norm(e);
                FP16_xtr_A(&t,&cu,&cv,&cumv,&cum2v);
                FP16_conj(&cum2v,&cumv);
                FP16_copy(&cumv,&cv);
                FP16_copy(&cv,&cu);
                FP16_copy(&cu,&t);
            }
            else if (BIG_parity(d)==0)
            {
                BIG_shr(d,1);
                FP16_conj(r,&cum2v);
                FP16_xtr_A(&t,&cu,&cumv,&cv,r);
                FP16_xtr_D(&cum2v,&cumv);
                FP16_copy(&cumv,&t);
                FP16_xtr_D(&cu,&cu);
            }
            else if (BIG_parity(e)==1)
            {
                BIG_sub(d,d,e);
                BIG_norm(d);
                BIG_shr(d,1);
                FP16_xtr_A(&t,&cu,&cv,&cumv,&cum2v);
                FP16_xtr_D(&cu,&cu);
                FP16_xtr_D(&cum2v,&cv);
                FP16_conj(&cum2v,&cum2v);
                FP16_copy(&cv,&t);
            }
            else
            {
                BIG_copy(w,d);
                BIG_copy(d,e);
                BIG_shr(d,1);
                BIG_copy(e,w);
                FP16_xtr_D(&t,&cumv);
                FP16_conj(&cumv,&cum2v);
                FP16_conj(&cum2v,&t);
                FP16_xtr_D(&t,&cv);
                FP16_copy(&cv,&cu);
                FP16_copy(&cu,&t);
            }
        }
        if (BIG_comp(d,e)<0)
        {
            BIG_imul(w,d,4);
            BIG_norm(w);
            if (BIG_comp(e,w)<=0)
            {
                BIG_sub(e,e,d);
                BIG_norm(e);
                FP16_xtr_A(&t,&cu,&cv,&cumv,&cum2v);
                FP16_copy(&cum2v,&cumv);
                FP16_copy(&cumv,&cu);
                FP16_copy(&cu,&t);
            }
            else if (BIG_parity(e)==0)
            {
                BIG_copy(w,d);
                BIG_copy(d,e);
                BIG_shr(d,1);
                BIG_copy(e,w);
                FP16_xtr_D(&t,&cumv);
                FP16_conj(&cumv,&cum2v);
                FP16_conj(&cum2v,&t);
                FP16_xtr_D(&t,&cv);
                FP16_copy(&cv,&cu);
                FP16_copy(&cu,&t);
            }
            else if (BIG_parity(d)==1)
            {
                BIG_copy(w,e);
                BIG_copy(e,d);
                BIG_sub(w,w,d);
                BIG_norm(w);
                BIG_copy(d,w);
                BIG_shr(d,1);
                FP16_xtr_A(&t,&cu,&cv,&cumv,&cum2v);
                FP16_conj(&cumv,&cumv);
                FP16_xtr_D(&cum2v,&cu);
                FP16_conj(&cum2v,&cum2v);
                FP16_xtr_D(&cu,&cv);
                FP16_copy(&cv,&t);
            }
            else
            {
                BIG_shr(d,1);
                FP16_conj(r,&cum2v);
                FP16_xtr_A(&t,&cu,&cumv,&cv,r);
                FP16_xtr_D(&cum2v,&cumv);
                FP16_copy(&cumv,&t);
                FP16_xtr_D(&cu,&cu);
            }
        }
    }
    FP16_xtr_A(r,&cu,&cv,&cumv,&cum2v);
    for (i=0; i<f2; i++)	FP16_xtr_D(r,r);
    FP16_xtr_pow(r,r,d);
}

#endif



/*
int main(){
		FP2 w0,w1,f;
		FP8 w,t;
		FP8 c1,c2,c3,c4,cr;
		BIG a,b;
		BIG e,e1,e2;
		BIG p,md;


		BIG_rcopy(md,Modulus);
		//Test w^(P^4) = w mod p^2
		BIG_zero(a); BIG_inc(a,27);
		BIG_zero(b); BIG_inc(b,45);
		FP2_from_BIGs(&w0,a,b);

		BIG_zero(a); BIG_inc(a,33);
		BIG_zero(b); BIG_inc(b,54);
		FP2_from_BIGs(&w1,a,b);

		FP8_from_FP2s(&w,&w0,&w1);
		FP8_reduce(&w);

		printf("w= ");
		FP8_output(&w);
		printf("\n");


		FP8_copy(&t,&w);


		BIG_copy(p,md);
		FP8_pow(&w,&w,p);

		printf("w^p= ");
		FP8_output(&w);
		printf("\n");
//exit(0);

		BIG_rcopy(a,CURVE_Fra);
		BIG_rcopy(b,CURVE_Frb);
		FP2_from_BIGs(&f,a,b);

		FP8_frob(&t,&f);
		printf("w^p= ");
		FP8_output(&t);
		printf("\n");

		FP8_pow(&w,&w,p);
		FP8_pow(&w,&w,p);
		FP8_pow(&w,&w,p);
		printf("w^p4= ");
		FP8_output(&w);
		printf("\n");

// Test 1/(1/x) = x mod p^4
		FP8_from_FP2s(&w,&w0,&w1);
		printf("Test Inversion \nw= ");
		FP8_output(&w);
		printf("\n");

		FP8_inv(&w,&w);
		printf("1/w mod p^4 = ");
		FP8_output(&w);
		printf("\n");

		FP8_inv(&w,&w);
		printf("1/(1/w) mod p^4 = ");
		FP8_output(&w);
		printf("\n");

		BIG_zero(e); BIG_inc(e,12);



	//	FP8_xtr_A(&w,&t,&w,&t,&t);
		FP8_xtr_pow(&w,&w,e);

		printf("w^e= ");
		FP8_output(&w);
		printf("\n");


		BIG_zero(a); BIG_inc(a,37);
		BIG_zero(b); BIG_inc(b,17);
		FP2_from_BIGs(&w0,a,b);

		BIG_zero(a); BIG_inc(a,49);
		BIG_zero(b); BIG_inc(b,31);
		FP2_from_BIGs(&w1,a,b);

		FP8_from_FP2s(&c1,&w0,&w1);
		FP8_from_FP2s(&c2,&w0,&w1);
		FP8_from_FP2s(&c3,&w0,&w1);
		FP8_from_FP2s(&c4,&w0,&w1);

		BIG_zero(e1); BIG_inc(e1,3331);
		BIG_zero(e2); BIG_inc(e2,3372);

		FP8_xtr_pow2(&w,&c1,&w,&c2,&c3,e1,e2);

		printf("c^e= ");
		FP8_output(&w);
		printf("\n");


		return 0;
}
*/
