int CWorCCW(const Vector3f* pPos, int nCounts )
{
	if(NULL == pPos || nCounts <= 2)
		return 0;

	float fMinZ = FLT_MAX;

	for (int i = 0 ; i < nCounts; ++i)
	{
		if(pPos[i].z < fMinZ)
			fMinZ = pPos[i].z;
	}

	float fArea = 0.0f;

	for (int i = 0 ; i < nCounts ; ++i)
	{
		fArea += (pPos[i].z + pPos[(i+1)%nCounts].z - fMinZ * 2) * (pPos[(i+1)%nCounts].x - pPos[i].x);
	}

	if(fArea == 0.0f)
		return 0;

	return fArea > 0 ? 1 : -1;
}