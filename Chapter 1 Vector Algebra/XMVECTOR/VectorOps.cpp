
#include <windows.h> // for FLOAT definition
#include <directxmath.h>
#include <iostream>

using namespace std;
using namespace DirectX;

// Overload the  "<<" operators so that we can use cout to 
// output XMVECTOR objects.
ostream& operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z <<  ", " << dest.w << ")";
	return os;
}
 
void XMVectorTest_1()
{
    XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR u = XMVectorSet(1.0f, 2.0, 3.0f, 0.0f);
    XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0, 0.0f);
    XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);

    cout << "XMVectorTest_1: Test Vector Functions -------------------------- " << endl;
    cout << "XMVector(n)                          = " << n << endl;
    cout << "XMVector(u)                          = " << u << endl;
    cout << "XMVector(v)                          = " << v << endl;
    cout << "XMVector(w)                          = " << w << endl;

    cout << "Vector Addition            u + v     = " << u + v << endl;
    cout << "Vector Subtraction         u - v     = " << u - v << endl;
    cout << "Vector Multiplication      10.0f * u = " << 10.0f * u << endl;
    cout << "Vector Magnitude/Length    |u|       = " << XMVector3Length(u) << endl;
    cout << "Appro Magnitude/Length     |u|       ~ " << XMVector3LengthEst(u) << endl;
    cout << "Vector Normalize           u/|u|     = " << XMVector3Normalize(u) << endl;
    cout << "Approx Normalize           u/|u|     ~ " << XMVector3NormalizeEst(u) << endl;
    cout << "Vector Dot Product         u * v     = " << XMVector3Dot(u, v) << endl;
    cout << "Vector Cross Product       u x v     = " << XMVector3Cross(u, v) << endl;

    // Find proj_n(w) and perp_n(w)
    XMVECTOR projW;
    XMVECTOR perpW;
    XMVector3ComponentsFromNormal(&projW, &perpW, w, n);
    cout << "projW = " << projW << endl;
    cout << "perpW = " << perpW << endl;

    // Does projW + perpW == w?
    bool equal = XMVector3Equal(projW + perpW, w) != 0;
    bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;
    cout << "projW + perpW == w = " << equal << endl;
    cout << "projW + perpW != w = " << notEqual << endl;

    // The angle between projW and perpW should be 90 degrees.
    XMVECTOR angleVec;
    float angleRadians, angleDegrees;
    angleVec = XMVector3AngleBetweenVectors(projW, perpW);
    angleRadians = XMVectorGetX(angleVec);
    angleDegrees = XMConvertToDegrees(angleRadians);
    cout << "angle = " << angleDegrees << endl;

    angleVec = XMVector3AngleBetweenVectors(w, n);
    angleRadians = XMVectorGetX(angleVec);
    angleDegrees = XMConvertToDegrees(angleRadians);
    cout << "angle = " << angleDegrees << endl;
}

const float Epsilon = 0.001f;
bool Equals(float lhs, float rhs)
{
    // Is the distance between lhs and rhs less than EPSILON?
    return fabs(lhs - rhs) < Epsilon ? true : false;
}

// Note: For checking vectors (which has floats as components) use: 
// XMFINLINE BOOL XMVector3NearEqual(FXMVECTOR U, FXMVECTOR V, FXMVECTOR Epsilon);
// Returns
// abs(U.x – V.x) <= Epsilon.x &&
// abs(U.y – V.y) <= Epsilon.y &&
// abs(U.z – V.z) <= Epsilon.z

void XMVectorTest_2()
{
    cout << "XMVectorTest_2: Test Precision issues -------------------------- " << endl;

    XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
    XMVECTOR n = XMVector3Normalize(u);
    float LU = XMVectorGetX(XMVector3Length(n));

    // Mathematically, the length should be 1. Is it numerically?
    cout << "Length = " << LU << endl;
    if (LU == 1.0f)
        cout << "Direct Compare: Length 1" << endl;
    else
        cout << "Direct Compare: Length not 1" << endl;
    if (Equals(LU, 1.0f))
        cout << "Epsilon Check : Length 1" << endl;
    else
        cout << "Epsilon Check Length not 1" << endl;

    // Raising 1 to any power should still be 1. Is it?
    float powLU = powf(LU, 1.0e6f);
    cout << "Raising 1 to any power should still be 1. Is it?" << endl;
    cout << "LU^(10^6) = " << powLU << endl;

    u = XMVectorSet(3.34334f, 2.323232f, 1.11212f, 0.0f);
    XMVECTOR unorm_exact = XMVector3Normalize(u);
    XMVECTOR unorm_approx = XMVector3NormalizeEst(u);
    cout << "Normal exact  = " << unorm_exact << endl;
    cout << "Normal approx = " << unorm_approx << endl;
    if (XMVector3NearEqual(unorm_exact, unorm_approx, XMVectorSet(Epsilon, Epsilon, Epsilon, Epsilon)))
        cout << "Vector Epsilon Check : Equal" << endl;
    else
        cout << "Vectoe Epsilon Check : Unequal" << endl;

}

int main(void)
{
	cout.setf(ios_base::boolalpha);

	// Check support for SSE2 (Pentium4, AMD K8, and above).
	if( !XMVerifyCPUSupport() )
	{
		cout << "DIRECTX math not supported" << endl;
		return 0;
	}
	
	XMVECTOR p = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
	XMVECTOR q = XMVectorSet(2.0f, -0.5f, 0.5f, 0.1f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 4.0f, 8.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 2.5f);
	XMVECTOR w = XMVectorSet(0.0f, XM_PIDIV4, XM_PIDIV2, XM_PI);

	cout << "XMVectorAbs(v)                 = " << XMVectorAbs(v) << endl;
	cout << "XMVectorCos(w)                 = " << XMVectorCos(w) << endl;
	cout << "XMVectorLog(u)                 = " << XMVectorLog(u) << endl;
	cout << "XMVectorExp(p)                 = " << XMVectorExp(p) << endl;
	
	cout << "XMVectorPow(u, p)              = " << XMVectorPow(u, p) << endl;
	cout << "XMVectorSqrt(u)                = " << XMVectorSqrt(u) << endl;
	
	cout << "XMVectorSwizzle(u, 2, 2, 1, 3) = " 
		<< XMVectorSwizzle(u, 2, 2, 1, 3) << endl;
	cout << "XMVectorSwizzle(u, 2, 1, 0, 3) = " 
		<< XMVectorSwizzle(u, 2, 1, 0, 3) << endl;

	cout << "XMVectorMultiply(u, v)         = " << XMVectorMultiply(u, v) << endl;
	cout << "XMVectorSaturate(q)            = " << XMVectorSaturate(q) << endl;
	cout << "XMVectorMin(p, v)              = " << XMVectorMin(p, v) << endl;
	cout << "XMVectorMax(p, v)              = " << XMVectorMax(p, v) << endl;

	XMVectorTest_1();
	XMVectorTest_2();

	return 0;
}
