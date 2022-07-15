#pragma once

#include <complex>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <QComboBox>
#include <QString>

class Calculation
{
public:
    int GetPtsNum() { return this->_ptsNum; }
	int GetStrNum() { return this->_strNum; }
	double* GetF() { return this->_f; }
	double* GetP() { return this->_p; }
	double** GetSE() { return this->_SE; }

	void SetF(double* value) { this->_f = value; }
	void SetF(double value, int index) { this->_f[index] = value; }
	void SetP(double* value) { this->_p = value; }
	void SetP(double value, int index) { this->_p[index] = value; }
	void SetSE(double** value) { this->_SE = value; }
	void SetSE(double* value, int strIndex) { this->_SE[strIndex] = value; }
	void SetSE(double value, int strIndex, int colIndex) { this->_SE[strIndex][colIndex] = value; }

	// Determines the number of lines in the file and assigns a value to the _strNum.
    void CountStringsIn(QString fileName);

    void RecCalc(QString fileName, QComboBox* freqMult,
				 QString a, QString b, QString d,
				 QString minP, QString maxP, QString ptsNum);
    void CylCalc(QString fileName, QComboBox* freqMult,
				 QString d, QString l,
				 QString minP, QString maxP, QString ptsNum);
    
    void SaveCalc(QString fileName);
	void DeleteCalc();
	
private:
	int _ptsNum;
	int _strNum;
	double* _f;
	double* _p;
	double** _SE;
};
