#include "header.h"

Mat src, src_gray;
int maxCorners = 23;
int maxTrackbar = 100;
RNG rng(12345);
const float pi = 3.141592f;
int thresh = 100;

void parse(string & someString, vvs & attributeTable)
{
	int attributeCount = 0;
	vs vectorOfStrings;
	while (someString.length() != 0 && someString.find(',') != string::npos)
	{
		size_t pos;
		string singleAttribute;
		pos = someString.find_first_of(',');
		singleAttribute = someString.substr(0, pos);
		vectorOfStrings.push_back(singleAttribute);
		someString.erase(0, pos + 1);
	}
	vectorOfStrings.push_back(someString);
	attributeTable.push_back(vectorOfStrings);
	vectorOfStrings.clear();
}

vvs pruneTable(vvs& attributeTable, string& colName, string value)
{
	int i, j;
	vvs prunedTable;
	int column = -1;
	vs headerRow;
	for (i = 0; i < attributeTable[0].size(); i++) {
		if (attributeTable[0][i] == colName) {
			column = i;
			break;
		}
	}
	for (i = 0; i < attributeTable[0].size(); i++) {
		if (i != column) {
			headerRow.push_back(attributeTable[0][i]);
		}
	}
	prunedTable.push_back(headerRow);
	for (i = 0; i < attributeTable.size(); i++) {
		vs auxRow;
		if (attributeTable[i][column] == value) {
			for (j = 0; j < attributeTable[i].size(); j++) {
				if (j != column) {
					auxRow.push_back(attributeTable[i][j]);
				}
			}
			prunedTable.push_back(auxRow);
		}
	}
	return prunedTable;
}

node* buildDecisionTree(vvs& table, node* nodePtr, vvs& tableInfo)
{
	if (tableIsEmpty(table)) {
		return NULL;
	}
	if (isHomogeneous(table)) {
		nodePtr->isLeaf = true;
		nodePtr->label = table[1][table[1].size() - 1];
		return nodePtr;
	}
	else {
		string splittingCol = decideSplittingColumn(table);
		nodePtr->splitOn = splittingCol;
		int colIndex = returnColumnIndex(splittingCol, tableInfo);
		int iii;
		for (iii = 1; iii < tableInfo[colIndex].size(); iii++) {
			node* newNode = (node*) new node;
			newNode->label = tableInfo[colIndex][iii];
			nodePtr->childrenValues.push_back(tableInfo[colIndex][iii]);
			newNode->isLeaf = false;
			newNode->splitOn = splittingCol;
			vvs auxTable = pruneTable(table, splittingCol, tableInfo[colIndex][iii]);
			nodePtr->children.push_back(buildDecisionTree(auxTable, newNode, tableInfo));
		}
	}
	return nodePtr;
}

bool isHomogeneous(vvs& table)
{
	int i;
	int lastCol = table[0].size() - 1;
	string firstValue = table[1][lastCol];
	if (lastCol <= 0)
	{
		return true;
	}
	for (i = 1; i < table.size(); i++) {

		if (firstValue != table[i][lastCol]) {
			return false;
		}
	}
	return true;
}

vi countDistinct(vvs& table, int column)
{
	vs vectorOfStrings;
	vi counts;
	bool found = false;
	int foundIndex;
	for (int i = 1; i < table.size(); i++) {
		for (int j = 0; j < vectorOfStrings.size(); j++) {
			if (vectorOfStrings[j] == table[i][column]) {
				found = true;
				foundIndex = j;
				break;
			}
			else {
				found = false;
			}
		}
		if (!found) {
			counts.push_back(1);
			vectorOfStrings.push_back(table[i][column]);
		}
		else {
			counts[foundIndex]++;
		}
	}
	int sum = 0;
	for (int i = 0; i < counts.size(); i++) {
		sum += counts[i];
	}
	counts.push_back(sum);
	return counts;
}

string decideSplittingColumn(vvs& table)
{
	int column, i;
	double minEntropy = DBL_MAX;
	int splittingColumn = 0;
	vi entropies;
	for (column = 0; column < table[0].size() - 1; column++) {
		string colName = table[0][column];
		msi tempMap;
		vi counts = countDistinct(table, column);
		vd attributeEntropy;
		double columnEntropy = 0.0;
		for (i = 1; i <= table.size() - 1; i++) {
			double entropy = 0.0;
			if (tempMap.find(table[i][column]) != tempMap.end()) { 
				tempMap[table[i][column]]++;
			}else { 
				tempMap[table[i][column]] = 1;
				vvs tempTable = pruneTable(table, colName, table[i][column]);
   				vi classCounts = countDistinct(tempTable, tempTable[0].size() - 1);
				int j;
				for (j = 0; j < classCounts.size(); j++) {
					double temp = (double)classCounts[j];
					entropy -= (temp / classCounts[classCounts.size() - 1]) * (log(temp / classCounts[classCounts.size() - 1]) / log(2));
				}
				attributeEntropy.push_back(entropy);
				entropy = 0.0;
			}
		}
		for (i = 0; i < counts.size() - 1; i++) {
			columnEntropy += ((double)counts[i] * (double)attributeEntropy[i]);
		}
		columnEntropy = columnEntropy / ((double)counts[counts.size() - 1]);
		if (columnEntropy <= minEntropy) {
			minEntropy = columnEntropy;
			splittingColumn = column;
		}
	}
	return table[0][splittingColumn];
}

int returnColumnIndex(string& columnName, vvs& tableInfo)
{

	int i;
	for (i = 0; i < tableInfo.size(); i++) {
		if (tableInfo[i][0] == columnName) {
			return i;
		}
	}
	return -1;
}

bool tableIsEmpty(vvs& table)
{
	return (table.size() == 1);
}

void printDecisionTree(node* nodePtr, string spacing = "")
{		

	if(nodePtr == NULL) {
		return;
	}
	if (!nodePtr->children.empty()) {
		cout << spacing << "Atrybut:" << nodePtr->label << endl;
		cout << spacing  << "Warunek: " << nodePtr->splitOn << endl;
		int iii;
		for (iii = 0; iii < nodePtr->children.size(); iii++) {   
			printDecisionTree(nodePtr->children[iii], spacing + "|");
		}
		return;
        } else {
		cout << spacing << "Warunek: " << "etykieta = " << nodePtr->label << endl;
		cout << spacing << nodePtr->label << endl;
		return;
	}

}

string testDataOnDecisionTree(vs& singleLine, node* nodePtr, vvs& tableInfo, string defaultClass)
{
	string prediction;
	while (!nodePtr->isLeaf && !nodePtr->children.empty()) {
		int index = returnColumnIndex(nodePtr->splitOn, tableInfo);
		string value = singleLine[index];
		int childIndex = returnIndexOfVector(nodePtr->childrenValues, value);
		nodePtr = nodePtr->children[childIndex];
		if (nodePtr == NULL) {
			prediction = defaultClass;
			break;
		}
		prediction = nodePtr->label;
	}
	return prediction;
}

int returnIndexOfVector(vs& stringVector, string value)
{
	int i;
	for (i = 0; i < stringVector.size(); i++) {
		if (stringVector[i] == value) {
			return i;
		}
	}
	return -1;
}

vvs generateTableInfo(vvs& dataTable)
{
	vvs tableInfo;
	for (int i = 0; i < dataTable[0].size(); i++) {
		vs tempInfo;
		msi tempMap;
		for (int j = 0; j < dataTable.size(); j++) {
			if (tempMap.count(dataTable[j][i]) == 0) {
				tempMap[dataTable[j][i]] = 1;
				tempInfo.push_back(dataTable[j][i]);
			}
			else {
				tempMap[dataTable[j][i]]++;
			}
		}
		tableInfo.push_back(tempInfo);
	}
	return tableInfo;
}

string returnMostFrequentClass(vvs& dataTable)
{
	msi trainingClasses;           												
	for (int i = 1; i < dataTable.size(); i++) {
		if (trainingClasses.count(dataTable[i][dataTable[0].size() - 1]) == 0) {
			trainingClasses[dataTable[i][dataTable[0].size() - 1]] = 1;
		}
		else {
			trainingClasses[dataTable[i][dataTable[0].size() - 1]]++;
		}
	}
	msi::iterator mapIter;
	int highestClassCount = 0;
	string mostFrequentClass;
	for (mapIter = trainingClasses.begin(); mapIter != trainingClasses.end(); mapIter++) {
		if (mapIter->second >= highestClassCount) {
			highestClassCount = mapIter->second;
			mostFrequentClass = mapIter->first;
		}
	}
	return mostFrequentClass;
}


float distance(Point2f a, Point2f b)
{
	// Calculating distance
	return sqrt(pow(b.x - a.x, 2) +
		pow(b.y - a.y, 2) * 1.0);
}

string calculateHWRatio(vp points)
{

	float h, w;

	auto xExtremes = std::minmax_element(points.begin(), points.end(),
		[](const Point2f& lhs, const Point2f& rhs) {
			return lhs.x < rhs.x;
		});

	auto yExtremes = std::minmax_element(points.begin(), points.end(),
		[](const Point2f& lhs, const Point2f& rhs) {
			return lhs.y < rhs.y;
		});

	Point2f upperLeft(xExtremes.first->x, yExtremes.first->y);
	Point2f lowerRight(xExtremes.second->x, yExtremes.second->y);

	h = abs(upperLeft.y - lowerRight.y);
	w = abs(lowerRight.x - upperLeft.x);

	int result = round(h / w);
	
	if (result == 1) {
		return "=";
	}

	return "!=";

}

vp getGoodFeaturesToTrack()
{
	vp corners;
	double qualityLevel = 0.3;
	double minDistance = 50;
	int blockSize = 3, gradientSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	goodFeaturesToTrack(src_gray,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		gradientSize,
		useHarrisDetector,
		k);

	return corners;
}

double getClockwiseAngle(Point2f p)
{
	double angle = 0.0;

	angle = -1 * atan2(p.x, -1 * p.y);
	return angle;
}

bool comparePoints(Point2f p1, Point2f p2)
{
	return getClockwiseAngle(p1) < getClockwiseAngle(p2);
}

float getAngleABC(Point2f a, Point2f b, Point2f c)
{
	Point2f ab = { b.x - a.x, b.y - a.y };
	Point2f cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return (float)floor(alpha * 180. / pi + 0.5);
}

int getParallelCnt(vector<Point2f> corners)
{
	int cnt = 0;
	Point2f A, B, Ap, Bp;
	for (size_t i = 0; i < corners.size(); i++)
	{
		A = corners[i];
		if ((i + 1) > corners.size() - 1) {
			B = corners[0];
		}
		else {
			B = corners[i + 1];
		}
		for (size_t n = 0; n < corners.size(); n++)
		{
			Ap = corners[n];
			if ((n + 1) > corners.size() - 1) {
				Bp = corners[0];
			}
			else {
				Bp = corners[n + 1];
			}

			float distA, distB;
			distA = distance(A, Ap);
			distB = distance(B, Bp);
			float delta = 0.01;
			if (abs(distA - distB) < delta && distA && distB)
			{
				cnt++;
			}
		}
	}

	return cnt / 2;
}

int getRightAnglesCnt(vp corners)
{
	int cnt = 0;
	for (int i = 0; i < corners.size(); i++)
	{
		Point2f O = corners[i];
		Point2f A;
		Point2f B;
		if (i == 0) {
			A = corners[corners.size() - 1];
		}
		else {
			A = corners[i - 1];
		}

		if ((i + 1) > corners.size() - 1) {
			B = corners[0];
		}
		else {
			B = corners[i + 1];
		}

		float angle = getAngleABC(A, O, B);
		if (angle < 95 && angle < 85) {
			cnt++;
		}

	}
	return cnt;

}

figure processImage(string imageToLearn) {
	figure f;
	src = imread(imageToLearn);
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	vector<Point2f> corners;
	corners = getGoodFeaturesToTrack();
	f.cornerCnt = to_string(corners.size());
	if (corners.size() > 4) {
		f.cornerCnt = ">4";
	}
	f.hWRatio = calculateHWRatio(corners);
	float sumX = 0;
	float sumY = 0;
	for (size_t i = 0; i < corners.size(); i++)
	{
		sumX += corners[i].x;
		sumY += corners[i].y;
	}
	Point2f center;
	center.x = sumX / corners.size();
	center.y = sumY / corners.size();
	for (size_t i = 0; i < corners.size(); i++)
	{
		corners[i].y = corners[i].y - center.y;
		corners[i].x = corners[i].x - center.x;
	}
	std::sort(corners.begin(), corners.end(), comparePoints);
	if (corners.size() > 4) {
		f.parallelCnt = to_string(0);
	}
	else {
		f.parallelCnt = to_string(getParallelCnt(corners));
	}
	f.rAngleCnt = to_string(getRightAnglesCnt(corners));
	if (getRightAnglesCnt(corners) > 4) {
		f.rAngleCnt = ">4";
	}
	f.label = getFileName(imageToLearn);

	return f;
}

string getFileName(string strPath)
{
	fs::path pathObj(strPath);
	string filename = pathObj.stem().string();
	filename.resize(filename.size() - 2);
	return filename;
	
}

vs figureToVS(figure fig) {
	vs processedImage;

	processedImage.push_back(fig.cornerCnt);
	processedImage.push_back(fig.hWRatio);
	processedImage.push_back(fig.parallelCnt);
	processedImage.push_back(fig.rAngleCnt);
	processedImage.push_back(fig.label);

	return processedImage;
}

vs getHeaders() {
	vs processedImage;

	processedImage.push_back("cornerCnt");
	processedImage.push_back("hwRatio");
	processedImage.push_back("parallelCnt");
	processedImage.push_back("rAngleCnt");
	processedImage.push_back("label");

	return processedImage;
}

vs getAllFilesNamesWithinFolder(string folder) {
	vs data;
	for (const auto& entry : fs::directory_iterator(folder)) {
		data.push_back(entry.path().string());
	}

	return data;
}



