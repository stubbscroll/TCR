__kernel void symm(
	__global float* outputC, int dM, int dN, __global float* inputA, __global float* inputB)
{
		int tx = get_global_id(0); //j
		int ty = get_global_id(1); //i

		float sum = 0.0f;

		for(int k = 0; k < dM; k++)
		{
			float elementA = inputA[tx*dM+k];
            float elementB = inputB[k*dN+ty];
            sum += elementA * elementB;
		}
        outputC[tx*dN+ty] = sum;

}
