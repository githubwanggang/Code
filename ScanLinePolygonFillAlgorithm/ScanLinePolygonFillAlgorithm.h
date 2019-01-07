#pragma once

#include <vector>


const int MIN_EDGE_COUNT = 3;

class Point
{
public:
	int X;
	int Y;
	Point(int InX = 0, int InY = 0)
		: X(InX)
		, Y(InY)
	{
	
	}
};

class Line
{
public:
	Point P1;
	Point P2;
	Line(Point InP1, Point InP2)
		: P1(InP1)
		, P2(InP2)
	{
		
	}
};

std::vector<Line> InitEdges(std::vector<Point> Ploygon, int &OutPointYMin, int &OutPointYMax)
{
	std::vector<Line> EdgesVector;

	if (Ploygon.size() < MIN_EDGE_COUNT)
	{
		return EdgesVector;
	}
	int PointYMin = INT_MAX;
	int PointYMax = INT_MIN;
	for (int PointIndex = 1; PointIndex < Ploygon.size(); ++PointIndex)
	{
		if (Ploygon.at(PointIndex - 1).Y < PointYMin)
		{
			PointYMin = Ploygon.at(PointIndex - 1).Y;
		}

		if (Ploygon.at(PointIndex - 1).Y > PointYMax)
		{
			PointYMax = Ploygon.at(PointIndex - 1).Y;
		}
		Line Current = Line(Ploygon.at(PointIndex - 1), Ploygon.at(PointIndex));
		EdgesVector.push_back(Current);
	}

	// 添加最后一个点和第一个点连成的边
	int MaxIndex = Ploygon.size() - 1;
	if (Ploygon.at(MaxIndex - 1).Y < PointYMin)
	{
		PointYMin = Ploygon.at(MaxIndex - 1).Y;
	}
	if (Ploygon.at(MaxIndex - 1).Y > PointYMax)
	{
		PointYMax = Ploygon.at(MaxIndex - 1).Y;
	}		
	Line Current = Line(Ploygon.at(MaxIndex), Ploygon.at(0));
	EdgesVector.push_back(Current);

	OutPointYMin = PointYMin;
	OutPointYMax = PointYMax;
	return EdgesVector;
}

bool FindIntersectPoint(int X1, int Y1, int X2, int Y2, int Y, int &X)
{
	if (Y2 == Y1)
		return false;
	X = (X2 - X1)*(Y - Y1) / (Y2 - Y1) + X1;
	bool bInsideEdgeX;
	bool bInsideEdgeY;
	if (X1 < X2)
		bInsideEdgeX = (X1 <= X) && (X <= X2);
	else
		bInsideEdgeX = (X2 <= X) && (X <= X1);

	if (Y1 < Y2)
		bInsideEdgeY = (Y1 <= Y) && (Y <= Y2);
	else
		bInsideEdgeY = (Y2 <= Y) && (Y <= Y1);
	return bInsideEdgeX && bInsideEdgeY;
}

std::vector<Point> ScanLineFill(std::vector<Line> EdgesVector, int PointYMin, int PointYMax)
{
	std::vector<Point> FillPositionVector;
	for (int IndexY = PointYMin; IndexY <= PointYMax; ++IndexY)
	{
		std::vector<Point> IntersectPointVector;
		int IntersectX = 0;
		int EdgesCount = EdgesVector.size();
		for (int IndexEdge = 0; IndexEdge != EdgesCount; ++IndexEdge)
		{
			Point P1 = EdgesVector.at(IndexEdge).P1;
			Point P2 = EdgesVector.at(IndexEdge).P2;

			bool bInsideEdge = FindIntersectPoint(P1.X, P1.Y, P2.X, P2.Y, IndexY, IntersectX);
			if (bInsideEdge)
			{
				Point IntersectPoint(IntersectX, IndexY);
				if (P1.Y > P2.Y)
				{
					if (IntersectPoint.Y == P1.Y)
					{
						continue;
					}
				}
				else
				{
					if (P1.Y < P2.Y)
					{
						if (IntersectPoint.Y == P2.Y)
						{
							continue;
						}
					}
				}
				IntersectPointVector.push_back(IntersectPoint);
			}
		}

		int IntersectCount = IntersectPointVector.size();
		Point SwapPoint;
		for (int IntersectIndex = 0; IntersectIndex < IntersectCount - 1; ++IntersectIndex)
		{
			for (int IntersectJ = IntersectIndex + 1; IntersectJ < IntersectCount; ++IntersectJ)
			{
				if (IntersectPointVector[IntersectIndex].X > IntersectPointVector[IntersectJ].X)
				{
					SwapPoint = IntersectPointVector[IntersectIndex];
					IntersectPointVector[IntersectIndex] = IntersectPointVector[IntersectJ];
					IntersectPointVector[IntersectJ] = SwapPoint;
				}
			}
		}

		for (int IntersectIndex = 1; IntersectIndex < IntersectCount; IntersectIndex += 2)
		{
			Point StartPoint = IntersectPointVector.at(IntersectIndex - 1);
			Point EndPoint = IntersectPointVector.at(IntersectIndex);
			for (int PointIndex = 0; PointIndex != EndPoint.X - StartPoint.X + 1; ++PointIndex)
			{
				Point FillPoint = Point(StartPoint.X + PointIndex, StartPoint.Y);
				FillPositionVector.push_back(FillPoint);
			}
		}
	}
	return FillPositionVector;
}

std::vector<Point> GetFillPositionVector(std::vector<Point> Ploygon)
{
	int PointYMin = INT_MAX;
	int PointYMax = INT_MIN;
	std::vector<Line> EdgesVector = InitEdges(Ploygon, PointYMin, PointYMax);
	std::vector<Point> FillPositionVector = ScanLineFill(EdgesVector, PointYMin, PointYMax);

	return FillPositionVector;
}

std::vector<Point> GetFillQPositionVector(std::vector<QPoint> Ploygon)
{
	std::vector<Point> MPloygon;
	for(int PointIndex = 0; PointIndex != Ploygon.size(); ++PointIndex)
	{
		QPoint TempPoint = Ploygon[PointIndex];
		MPloygon.push_back(Point(TempPoint.x(), TempPoint.y()));
	}
	return GetFillPositionVector(MPloygon);
}

