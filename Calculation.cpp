#include "Calculation.h"

const int c = 299792458;
const double pi = 3.141592;
const double v0 = 1.0;
const double z0 = 120.0 * pi;
const double zIn = 50.0;
const std::complex <double> jj(0.0, 1.0);

// The roots of the Bessel function
const double bfr[20] = {1.841, 2.405, 3.054, 3.832, 3.832,
                        4.201, 5.135, 5.331, 5.520, 6.380,
                        6.706, 7.016, 7.016, 8.015, 8.417,
                        8.536, 8.650, 9.760, 9.969, 10.173};

void Calculation::CountStringsIn(QString fileName)
{
    std::ifstream file(fileName.toStdString());
    if (file.is_open())
    {
        file.seekg(0);
        file.clear();
        this->_strNum = 0;
        std::string temp;
        while (!file.eof())
        {
            file >> temp;
            this->_strNum++;
        }
		this->_strNum /= 2;
    }
    file.close();
}

void Calculation::RecCalc(QString fileName, QComboBox* freqMultValue,
                          QString aValue, QString bValue, QString dValue,
                          QString minPValue, QString maxPValue, QString ptsNumValue)
{
	double a = aValue.toDouble() / 1000;
	double b = bValue.toDouble() / 1000;
	double d = dValue.toDouble() / 1000;
	double minP = minPValue.toDouble() / 1000;
	double maxP = maxPValue.toDouble() / 1000;
	
	int freqMult = 1;
	switch (freqMultValue->currentIndex())
	{
	case 0:
	    freqMult = pow(10, 0);
	    break;
	case 1:
	    freqMult = pow(10, 3);
	    break;
	case 2:
	    freqMult = pow(10, 6);
	    break;
	case 3:
	    freqMult = pow(10, 9);
	    break;
	}
	this->_ptsNum = ptsNumValue.toInt();
	
	this->CountStringsIn(fileName);
	std::ifstream file(fileName.toStdString());
	if (file.is_open())
	{
	    file.seekg(0);
	    file.clear();
	    double* s11 = new double[this->GetStrNum()];
	    this->SetF(new double[this->GetStrNum()]);
	    std::string temp;
	    for (int i = 0; i < this->GetStrNum(); i++)
	    {
	        file >> std::setprecision(15) >> temp;
	        for (unsigned int j = 0; j < temp.length(); j++)
	        {
	            if (temp[j] == ',')
	                temp[j] = '.';
	        }
	        this->SetF(stod(temp) * freqMult, i);
			
	        file >> std::setprecision(14) >>temp;
	        for (unsigned int j = 0; j < temp.length(); j++)
	        {
	            if (temp[j] == ',')
	                temp[j] = '.';
	        }
	        s11[i] = std::stod(temp);
	    }
	
	    double pStep = (maxP - minP) / (this->GetPtsNum() - 1.0);
	    this->SetP(new double[this->GetPtsNum()]);
	    for (int i = 0; i < this->GetPtsNum(); i++)
	    {
	        this->SetP(minP, i);
	        minP += pStep;
	    }
	
	    this->SetSE(new double* [this->GetStrNum()]);
	    for (int i = 0; i < this->GetStrNum(); i++)
	        this->SetSE(new double[this->GetPtsNum()], i);
	
	    std::complex <double> lambda, k0, z, zg, kg, zgtemp, kgtemp,
							  zsc, zap, z1, v1, z2, v2, z3, vp, vpp;
	
	    for (int i = 0; i < this->GetPtsNum(); i++)
	    {
	        for (int j = 0; j < this->GetStrNum(); j++)
	        {
	            // Calculation of aperture resistance.
	            lambda = c / this->GetF()[j];
	            k0 = 2 * pi / lambda;
	            z = 2.0 * zIn * s11[j] / (1.0 - s11[j]);
	            zgtemp = z0 / (sqrt(1.0 - pow(lambda / (2 * a), 2)));
	            kgtemp = k0 * (sqrt(1.0 - pow(lambda / (2 * a), 2)));
	            zsc = jj * zgtemp * tan(kgtemp * d);
	            zap = z * zsc / (zsc - z);
	
	            // Converting source to aperture.
	            v1 =  v0 * zap / (z0 + zap);
	            z1 = zIn * zap / (z0 + zap);
	
	            // Calculation of voltages in the equivalent circuit.
	            vpp = 0;
	            zg = z0 / sqrt(1.0 - pow(lambda / (2 * a), 2));
				kg = k0 * sqrt(1.0 - pow(lambda / (2 * a), 2));
	            v2 = v1 / (cos(kg * this->GetP()[i]) + jj * (z1 / zg) *
					       sin(kg * this->GetP()[i]));
	            z2 = (z1 + jj * zg * tan(kg * this->GetP()[i])) /
					 (1.0 + jj * (z1 / zg) * tan(kg * this->GetP()[i]));
	            z3 = jj * zg * tan(kg * (d - this->GetP()[i]));
	            vp = v2 * z3 / (z2 + z3);
	            vpp += vp;
	
				// Calculation of the shielding effectiveness.
	            this->SetSE(-20.0 * log10(abs(v0 / 2.0 * vpp)), j, i);
	        }
	    }
	
	    delete[] s11;
	}
}

void Calculation::CylCalc(QString fileName, QComboBox* freqMultValue,
                          QString dValue_2, QString lValue,
                          QString minPValue, QString maxPValue, QString ptsNumValue)
{
    double d = dValue_2.toDouble() / 1000;
    double l = lValue.toDouble() / 1000;
    double minP = minPValue.toDouble() / 1000;
    double maxP = maxPValue.toDouble() / 1000;

    int freqMult = 1;
    switch (freqMultValue->currentIndex())
    {
    case 0:
        freqMult = pow(10, 0);
        break;
    case 1:
        freqMult = pow(10, 3);
        break;
    case 2:
        freqMult = pow(10, 6);
        break;
    case 3:
        freqMult = pow(10, 9);
        break;
    }
    this->_ptsNum = ptsNumValue.toInt();

    this->CountStringsIn(fileName);
    std::ifstream file(fileName.toStdString());
    if (file.is_open())
    {
        file.seekg(0);
        file.clear();
        double* s11 = new double[this->GetStrNum()];
        this->SetF(new double[this->GetStrNum()]);
        std::string temp;
        for (int i = 0; i < this->GetStrNum(); i++)
        {
            file >> std::setprecision(15) >> temp;
            for (unsigned int j = 0; j < temp.length(); j++)
            {
                if (temp[j] == ',')
                    temp[j] = '.';
            }
            this->SetF(stod(temp) * freqMult, i);

            file >> std::setprecision(14) >>temp;
            for (unsigned int j = 0; j < temp.length(); j++)
            {
                if (temp[j] == ',')
                    temp[j] = '.';
            }
            s11[i] = std::stod(temp);
        }

        double pStep = (maxP - minP) / (this->GetPtsNum() - 1.0);
        this->SetP(new double[this->GetPtsNum()]);
        for (int i = 0; i < this->GetPtsNum(); i++)
        {
            this->SetP(minP, i);
            minP += pStep;
        }

        this->SetSE(new double* [this->GetStrNum()]);
        for (int i = 0; i < this->GetStrNum(); i++)
            this->SetSE(new double[this->GetPtsNum()], i);

        double r = d / 2;
        std::complex <double> lam, lamc, lamtemp = 2.0 * pi * r / 1.841;
        std::complex <double> z, zg, kg, zgtemp, kgtemp, zsc, zap, z1, v1, z2, v2, z3, vp, vpp;

        for (int i = 0; i < this->GetPtsNum(); i++)
        {
            for (int j = 0; j < this->GetStrNum(); j++)
            {
				// Calculation of aperture resistance.
				lam = c / this->GetF()[j];
                z = 2.0 * zIn * s11[j] / (1.0 - s11[j]);
                zgtemp = z0 / (sqrt(1.0 - pow(lam / lamtemp, 2)));
                kgtemp = (2.0 * pi / lam) * sqrt(1.0 - pow(lam / lamtemp, 2));
                zsc = jj * zgtemp * tan(kgtemp * l);
                zap = z * zsc / (zsc - z);

				// Converting source to aperture.
                v1 = v0 * zap / (z0 + zap);
                z1 = zIn * zap / (z0 + zap);

				// Calculation of voltages in the equivalent circuit.				
                vpp = 0;
                for (int n = 0; n < 20; n++)
                {
                    lamc = 2.0 * pi * r / bfr[n];
                    zg = z0 / (sqrt(1.0 - pow(lam / lamc, 2)));
                    kg = (2.0 * pi / lam) * sqrt(1.0 - pow(lam / lamc, 2));
                    v2 = v1 / (cos(kg * this->GetP()[i]) + jj * (z1 / zg) * sin(kg * this->GetP()[i]));
                    z2 = (z1 + jj * zg * tan(kg * this->GetP()[i])) / (1.0 + jj * (z1 / zg) * tan(kg * this->GetP()[i]));
                    z3 = jj * zg * tan(kg * (l - this->GetP()[i]));
                    vp = v2 * z3 / (z2 + z3);
                    vpp += vp;
                }

				// Calculation of the shielding effectiveness.
                this->SetSE(-20.0 * log10(abs(v0 / 2.0 * vpp)), j, i);
            }
        }
        delete[] s11;
    }
}

void Calculation::DeleteCalc()
{
    for (int i = 0; i < this->GetStrNum(); i++)
        delete[] this->GetSE()[i];
    delete[] this->GetP();
    delete[] this->GetF();
}

void Calculation::SaveCalc(QString fileName)
{
    std::ofstream myFile(fileName.toStdString());
    if (myFile.is_open())
    {
        myFile << std::setw(20) << "Frequency";
        myFile << std::setw(20) << "SE";
        myFile << std::setw(20) << "Observation point";
        myFile << std::endl;
        for (int i = 0; i < this->_ptsNum; i++)
        {
            for (int j = 0; j < this->_strNum; j++)
            {
                myFile << std::setw(20) << std::setprecision(15) << this->GetF()[j];
                myFile << std::setw(20) << std::setprecision(15) << this->GetSE()[j][i];
                myFile << std::setw(20) << std::setprecision(15) << this->GetP()[i];
                myFile << std::endl;
            }
            myFile << std::endl;
        }
        myFile.close();
    }
}
