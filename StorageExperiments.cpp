/*
	code to input data file and perform experiments
	inlcude experiments specification, generate data
	
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <time.h>

using namespace std;


//dataIns contain a vector of double value 
struct dataIns{
	vector<double> attrValues;
	double cla;
};
int main(int argc, char *argv[]){

	/*
		Random generated data to perform SGD and SCD
		the test case is would be a simple neural net binary classifier
		say if we have a n dimensional data then the model will have 
		n +1 dimension data and the class is random assigned as 0, 1
		sigmoid function is used to classify and during learning, cross- entropy error
		is used to minimize to perfect the model. 
		To make the classification meaningful
		and convergence will be reached, when classification is set to 0,
		all attributes of data instance in X will be randomly set to -0.5 to 0.5
		if the classfication is set to 1, then all attributes of data instance will
		be randomly set to -1.0 - -0.5 and 0.5 - 1.0; thus, 
		this is a dataset will be classified easily.
		There are several parameters need to be addressed
		numAttr, the number of attributes
		numTuples, the number of tuples,
		these two number will allow us to modify the size of dataset
		epochs, the number of whole dataset to be scanned
		alpha, learning rate, the stepsize led to convergence
	*/
	//number of attributes of each tuple
	int numAttr;
	//number of tuples
	int numTuples;
	//learning rate
	double alpha = 0.01;
	//number of scan of dataset
	int epochs = 5;

	if(argc != 3)
	{
		cout << "Please enter rgiht arguments!" << endl;
		return 0;
	}
	else
	{
		numAttr = atoi(argv[1]);
		numTuples = atoi(argv[2]);
	}
	cout << "numAttr: " << numAttr << endl;
	cout << "numTuples: " << numTuples << endl;
	//dataset contains all of the data.
	std::vector<dataIns> dataset;
	//generate data, initially store them in row-wise
	for(int i_tuple = 0; i_tuple < numTuples; i_tuple++)
	{
		//vector to store attributes value in tuples
		std::vector<double> insAttr;
		//random generate classification
		double cla = rand() > RAND_MAX / 2 ? 1.0 : 0.0;
		//randome generate attribute value
		double toPut;
		for(int i_attr = 0; i_attr < numAttr; i_attr++)
		{
			toPut = (double) rand() / (double) RAND_MAX;
			if(cla >= 0.5)
			{
				while(toPut < 0.5)
				{
					toPut = (double) rand() / (double) RAND_MAX;
				}
				double decor = rand() > RAND_MAX / 2 ? -1.0 : 0.0;
				toPut = toPut * decor;
			}
			else
			{	
				while(toPut >= 0.5)
				{
					toPut = (double) rand() / (double) RAND_MAX;
				}
				double decor = rand() > RAND_MAX / 2 ? -1.0 : 0.0;
				toPut = toPut * decor;
			}
			insAttr.push_back(toPut);
		}
		dataIns data;
		data.cla = cla;
		data.attrValues = insAttr;
		dataset.push_back(data);
	} 
	//SGD
	//initialize model;
	std::vector<double> model;
	for(int i_model = 0; i_model < numAttr + 1; i_model++)
	{
		double init = (double) (rand() % 100) / 10000;
		model.push_back(init);
	}
	clock_t t1 = clock();
	for(int i_epoch = 0; i_epoch < epochs; i_epoch++)
	{
		//scan tuples to verify the model
		for(dataIns dat : dataset)
		{
			//calculate classification
			double total;
			for(int i_attr = 0; i_attr < numAttr; i_attr++)
			{
				total = total + dat.attrValues.at(i_attr) * model.at(i_attr);
			}
			//bias node in neural network
			total = total + 1.0 * model.at(numAttr);
			double sig = 1.0 / (1.0 + exp((-1.0) * total));
			//update model
			for(int i_attr = 0; i_attr < numAttr; i_attr++)
			{
				//TODO
				//maybe need to change to square difference
				double deltaW = (-1.0) * alpha * (sig - dat.cla) * sig * (1.0 - sig) * dat.attrValues.at(i_attr);
				double toUpdate = model.at(i_attr);
				double newValue = toUpdate + deltaW;
				model.erase(model.begin() + i_attr);
				model.insert(model.begin() + i_attr, newValue);
			}
			//update weight related to bias node
			double deltaW = (-1.0) * alpha * (sig - 1.0) * sig * (1.0 - sig) * 1.0;
			double toUpdate = model.at(numAttr);
			double newValue = toUpdate + deltaW;
			model.erase(model.begin() + numAttr);
			model.insert(model.begin() + numAttr, newValue);
		}
	}
	t1 = clock() - t1;
	cout<<"SGD in row storage: " <<(double)t1 / CLOCKS_PER_SEC<<'\n';
	
	//SCD
	//reset the model
	model.clear();
	for(int i_model = 0; i_model < numAttr + 1; i_model++)
	{
		double init = (double) (rand() % 100) / 10000;
		model.push_back(init);
	}
	//for each epochs, random choose coordinate, which is attribute
	//calculate update formula, since only one coordinate was changed
	//need to scan one column to update the error.
	//initialize error
	double error = 0.0;
	std::vector<double> Output;
	for(dataIns dat : dataset)
	{
		double total = 0.0;
		for(int i_attr = 0; i_attr < numAttr; i_attr++)
		{
			total = total + dat.attrValues.at(i_attr) * model.at(i_attr);
		}
		//bias node in neural network
		total = total + 1.0 * model.at(numAttr);
		double sig = 1.0 / (1.0 + exp((-1.0) * total));
		Output.push_back(sig);
	}
	clock_t t2 = clock();
	for(int i_epoch = 0; i_epoch < epochs; i_epoch++)
	{
		for(int i_attr = 0; i_attr < numAttr + 1; i_attr++)
		{
			//choose the index of weight to update
			int indexToUpdate = rand() % numAttr;
			double deltaW = 0.0;
			//cout<<dataset.size();
			for(int i_tuple = 0; i_tuple < numTuples; i_tuple++)
			{
				double o = Output.at(i_tuple);
				deltaW = (-1.0) * (o - dataset.at(i_tuple).cla) * o * (1.0 - o) * dataset.at(i_tuple).attrValues.at(indexToUpdate);
			}
			double oldValue = model.at(indexToUpdate);
			double newValue = oldValue + deltaW;
			model.erase(model.begin() + indexToUpdate);
			model.insert(model.begin() + indexToUpdate, newValue);
			//updata output
			for(int i_tuple = 0; i_tuple < numTuples; i_tuple++)
			{
				double init = Output.at(i_tuple);
				double minusE = 1.0 / init - 1.0;
				double newE = minusE * exp((-1.0) * (deltaW * dataset.at(i_tuple).attrValues.at(indexToUpdate)));
				double newO = 1.0 / (1.0 + newE);
				Output.erase(Output.begin() + i_tuple);
				Output.insert(Output.begin() + i_tuple, newO);
			}
		}
	}
	t2 = clock() - t2;
	cout<<"SCD in row storage: " <<(double)t2 / CLOCKS_PER_SEC<<'\n';
	
	//transpose change dataset from row wise to column wise
	std::vector<dataIns> newDataset;
	for(int i_column = 0; i_column < numAttr; i_column++)
	{
		dataIns toStore;
		std::vector<double> values;
		for(dataIns dat : dataset)
		{
			values.push_back(dat.attrValues.at(i_column));
		}
		toStore.attrValues = values;
		newDataset.push_back(toStore);
	}
	//use a seperate vector to store classification, to be fair.
	std::vector<double> newCla;
	for(dataIns dat : dataset)
	{
		newCla.push_back(dat.cla);
	}
	std::vector<dataIns> toFree;
	dataset.swap(toFree);
	//reset the model
	model.clear();
	for(int i_model = 0; i_model < numAttr + 1; i_model++)
	{
		double init = (double) (rand() % 100) / 10000;
		model.push_back(init);
	}
	//SGD
	clock_t t3 = clock();
	for(int i_epoch = 0; i_epoch < epochs; i_epoch++)
	{
		for(int i_tuple = 0; i_tuple < numTuples; i_tuple++)
		{
			double total;
			for(int i_attr = 0; i_attr < numAttr; i_attr++)
			{
				total = total + newDataset.at(i_attr).attrValues.at(i_tuple) * model.at(i_attr);
			}
			//bias node in neural network
			total = total + 1.0 * model.at(numAttr);
			double sig = 1.0 / (1.0 + exp((-1.0) * total));
			for(int i_attr = 0; i_attr < numAttr; i_attr++)
			{
				//TODO
				//maybe need to change to square difference
				double deltaW = (-1.0) * alpha * (sig - newCla.at(i_tuple)) * sig * (1.0 - sig) * newDataset.at(i_attr).attrValues.at(i_tuple);
				double toUpdate = model.at(i_attr);
				double newValue = toUpdate + deltaW;
				model.erase(model.begin() + i_attr);
				model.insert(model.begin() + i_attr, newValue);
			}
			//update weight related to bias node
			double deltaW = (-1.0) * alpha * (sig - 1.0) * sig * (1.0 - sig) * 1.0;
			double toUpdate = model.at(numAttr);
			double newValue = toUpdate + deltaW;
			model.erase(model.begin() + numAttr);
			model.insert(model.begin() + numAttr, newValue);
		}
	}
	t3 = clock() - t3;
	cout<<"SGC in column storage: "<<(double)t3 / CLOCKS_PER_SEC<<'\n';
	//SCD
	model.clear();
	for(int i_model = 0; i_model < numAttr + 1; i_model++)
	{
		double init = (double) (rand() % 100) / 10000;
		model.push_back(init);
	}
	std::vector<double> newOut;
	for(int i_dat = 0; i_dat < numTuples; i_dat++)
	{
		double total = 0.0;
		for(int i_attr = 0; i_attr < numAttr; i_attr++)
		{
			total = total + newDataset.at(i_attr).attrValues.at(i_dat) * model.at(i_attr);
		}
		total = total + 1.0 * model.at(numAttr);
		double sig = 1.0 / (1.0 + exp((-1.0) * total));
		newOut.push_back(sig);
	}
	clock_t t4 = clock();
	for(int i_epoch = 0; i_epoch < epochs; i_epoch++)
	{
		for(int i_attr = 0; i_attr < numAttr + 1; i_attr++)
		{
			//choose the index of weight to update
			int indexToUpdate = rand() % numAttr;
			double deltaW = 0.0;
			for(int i_tuple = 0; i_tuple < numTuples; i_tuple++)
			{
				double o = newOut.at(i_tuple);
				deltaW = (-1.0) * (o - newCla.at(i_tuple)) * o * (1.0 - o) * newDataset.at(indexToUpdate).attrValues.at(i_tuple);
			}
			double oldValue = model.at(indexToUpdate);
			double newValue = oldValue + deltaW;
			model.erase(model.begin() + indexToUpdate);
			model.insert(model.begin() + indexToUpdate, newValue);
			for(int i_tuple = 0; i_tuple < numTuples; i_tuple++)
			{
				double init = newOut.at(i_tuple);
				double minusE = 1.0 / init - 1.0;
				double newE = minusE * exp((-1.0) * (deltaW * newDataset.at(indexToUpdate).attrValues.at(i_tuple)));
				double newO = 1.0 / (1.0 + newE);
				newOut.erase(newOut.begin() + i_tuple);
				newOut.insert(newOut.begin() + i_tuple, newO);
			}
		}
	}
	t4 = clock() - t4;
	cout<<"SCD in column storage: "<<(double)t4 / CLOCKS_PER_SEC<<'\n';
	return 1;
}