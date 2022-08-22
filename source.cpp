#include "header.h"

int main()
{

	vs imagesToLearn;
	vector<figure> processedImages;

	imagesToLearn = getAllFilesNamesWithinFolder("trainImages");

	for (size_t i = 0; i < imagesToLearn.size(); i++)
	{
		processedImages.push_back(processImage(imagesToLearn[i]));
	}

	vvs dataTable;			
	dataTable.push_back(getHeaders());

	for (size_t i = 0; i < processedImages.size(); i++)
	{
		dataTable.push_back(figureToVS(processedImages[i]));
	}

	vvs tableInfo = generateTableInfo(dataTable);
	node* root = new node;
	root->isLeaf = false;
	root = buildDecisionTree(dataTable, root, tableInfo);	
	printDecisionTree(root, "");
	string defaultClass = returnMostFrequentClass(dataTable);
	dataTable.clear();

	string imageToTest;

	imageToTest = "test01.jpg";
	vs processedTestImage = figureToVS(processImage(imageToTest));
	
	string prediction = testDataOnDecisionTree(processedTestImage, root, tableInfo, defaultClass);

	cout << "Przewidziana klasa = " << prediction << endl;


	system("pause");
	return 0;
}
