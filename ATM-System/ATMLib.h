#pragma once
#include <iostream>
#include <iomanip>

#include"MyFunctionLib.h"

#include"ATMLib.h"

#include <cstdio>
#include<vector>
#include<string.h>
#include<cctype>
#include<string>
#include<fstream>
#include<ctime>

using namespace std;
using namespace MyFunctionsLib;

namespace ATMLib
{
	const string FileName = R"(C:\BankData\Clients.txt)";

	struct sClient {
		string AccountNumber;
		string PinCode;
		string Name;
		string Phone;
		int AccountBalance = 0;
	};

	inline sClient CurrentClient;

	enum enAtmSystem
	{
		QuickWithdarw = 1,
		NormalWithdraw = 2,
		Deposit = 3,
		CheckBalance = 4,
		Logout = 5
	};

	void Login();
	void ShowCheckBalanceScreen();
	void ShowATMMainMenu();
	void ShowQucikWithdrawScreen();
	void ShowNormalWithdrawScreen();
	int IsAmountDivisibleBy5();
	vector<sClient> UpdateFile(string FileName, vector<sClient>& vClient);
	void ShowDepositScreen();



	void ExecuteATMMenuOption(int ChaiseNumber, vector<sClient>& vClient)
	{
		switch (ChaiseNumber)
		{
		case enAtmSystem::QuickWithdarw:
		{
			ShowQucikWithdrawScreen();
			break;

		}

		case enAtmSystem::NormalWithdraw:
		{
			ShowNormalWithdrawScreen();

			char Answer;
			int AmountBalance = IsAmountDivisibleBy5();

			if (AmountBalance < CurrentClient.AccountBalance)
			{
				Answer = ReadCharacter("\n\n\nAre you sure you want perform this transaction? y/n ? ");

				if (toupper(Answer) == 'Y')
				{
					CurrentClient.AccountBalance -= AmountBalance;
					UpdateFile(FileName, vClient);

					cout << "\n\nDone Successfully. New balance is: " << CurrentClient.AccountBalance << "\n\n\n";
					break;
				}
				break;
			}
			else
			{
				cout << "\nThe amount exceeds your balance, make another choice.\n\n\n";
					break;
			}
			
             
		}

		case enAtmSystem::Deposit:
		{
			ShowDepositScreen();
			
			int AmountBalance = ReadNumberPositive("Enter a positive Deposit Amount? ");

			char Answer = ReadCharacter("Enter you sure you want perform this transaction? y/n ? ");

			if (toupper(Answer) == 'Y')
			{
				CurrentClient.AccountBalance += AmountBalance;
				UpdateFile(FileName, vClient);
				cout << "Done Successfully. New balance is: " << CurrentClient.AccountBalance << "\n\n\n";
			}
			break;
			
		}

		case enAtmSystem::CheckBalance:
		{

			ShowCheckBalanceScreen();

			cout << "Your Balance is " << CurrentClient.AccountBalance << "\n\n\n";
			break;
		}

		case enAtmSystem::Logout:
		{
			system("cls");
			Login();
		}


		}
	}

	int IsAmountDivisibleBy5()
	{
		int AmountBalance;

		char Answer = 'y';
		do
		{
			AmountBalance = ReadNumberPositive("Enter an amount multiple of 5's ? ");

			if (AmountBalance % 5 == 0)
			{
				Answer = 'n';
			}

		} while (toupper(Answer) == 'Y');
		
		return AmountBalance;
	}

	void ShowLoginScreen()
	{
		cout << "---------------------------------------------\n";
		cout << "              Login Screen \n";
		cout << "---------------------------------------------\n\n";
	}

	void ShowCheckBalanceScreen()
	{
		cout << "=============================================\n";
		cout << "             Check Balance Screen \n";
		cout << "=============================================\n\n";
	}

	void ShowNormalWithdrawScreen()
	{
		cout << "=============================================\n";
		cout << "                Normal Withdraw Screen \n";
		cout << "=============================================\n\n";
	}

	void ShowDepositScreen()
	{
		cout << "=============================================\n";
		cout << "                   Deposit Screen \n";
		cout << "=============================================\n\n";
	}

	sClient ReadNewClient()
	{
		sClient Client;

		Client.PinCode = ReadString("\nEnter PinCode? ");
		Client.Name = ReadString("\nEnter Name? ");
		Client.Phone = ReadString("\nEnter Phone? ");
		Client.AccountBalance = ReadNumberPositive("\nEnter AccountBalance? ");

		return Client;
	}

	bool IsClientExistAccountNumberAndBinCode(vector<sClient>& vClients, string AccountNumber, string PinCode)
	{
		for (sClient& c : vClients)
		{
			if (c.AccountNumber == AccountNumber)
			{
				if (c.PinCode == PinCode)
				{
					return true;
				}
			}
		}
		return false;
	}

	sClient ResearchClintByAccountNumber(vector<sClient>& vClients, string AccountNumber)
	{
		for (sClient& c : vClients)
		{
			if (c.AccountNumber == AccountNumber)
			{
				return c;
			}
		}
	}

	vector<string> splitLine(string Line, string Spemeter = "#//#")
	{
		vector<string> Members;

		int pos = 0;

		while ((pos = Line.find(Spemeter)) != std::string::npos)
		{
			Members.push_back(Line.substr(0, pos));
			Line.erase(0, pos + Spemeter.length());
		}
		if (Line != "")
		{
			Members.push_back(Line);
		}
		return Members;
	}

	sClient ConvertLineToRecordClient(string Line)
	{
		sClient Client;

		vector<string> Members = splitLine(Line);

		Client.AccountNumber = Members[0];
		Client.PinCode = Members[1];
		Client.Name = Members[2];
		Client.Phone = Members[3];
		Client.AccountBalance = stoi(Members[4]);

		return Client;
	}

	string ConvertClientToLine(sClient Client, string Spemeter = "#//#")
	{
		string S1 = "";
		S1 += Client.AccountNumber + Spemeter;
		S1 += Client.PinCode + Spemeter;
		S1 += Client.Name + Spemeter;
		S1 += Client.Phone + Spemeter;
		S1 += to_string(Client.AccountBalance);

		return S1;
	}

	vector<sClient> LoadClientsDataFromFile(string FileName)
	{
		vector<sClient> vClients;
		sClient Client;

		ifstream MyFile;

		MyFile.open(FileName, ios::in);

		if (MyFile.is_open())
		{
			string Line;

			while (getline(MyFile, Line))
			{
				Client = ConvertLineToRecordClient(Line);
				vClients.push_back(Client);
			}

			MyFile.close();
		}

		return vClients;
	}

	void UpdateClientBalanceInVector(vector<sClient>& vClient)
	{
		for (sClient& c : vClient)
		{
			if (c.AccountNumber == CurrentClient.AccountNumber)
			{
				c.AccountBalance = CurrentClient.AccountBalance;
				break;
			}
		}
	}

	vector<sClient> UpdateFile(string FileName, vector<sClient>& vClient)
	{

		UpdateClientBalanceInVector(vClient);

		ofstream MyFile;

		MyFile.open(FileName, ios::out);

		if (MyFile.is_open())
		{
			for (sClient& c : vClient)
			{
				MyFile << ConvertClientToLine(c) << endl;
			}
			MyFile.close();
		}

		return vClient;
	}

	int UpdateClientBalance(int Number)
	{
		return CurrentClient.AccountBalance - Number;
	}

	void PerformQucikWithdrawOption(int ChoiseNumber, vector<sClient>& vClient)
	{

		if (ChoiseNumber == 9)
		{
			return;
		}
		int Amounts[8] = { 20, 50, 100, 200, 400, 600, 800, 1000 };

		char Answer;
		int Number = Amounts[ChoiseNumber - 1];

		if (Number > CurrentClient.AccountBalance)
		{
			cout << "\n\nThe amount exceeds your balance, make another choice\n\n";
			system("pause");
			system("cls");
			ShowQucikWithdrawScreen();
		}
		else
		{
			Answer = ReadCharacter("\n\nAre you sure you want perform this transaction? y/n? ");

			if (toupper(Answer) == 'Y')
			{
				CurrentClient.AccountBalance = UpdateClientBalance(Number);
				vClient = UpdateFile(FileName, vClient);
				cout << "\n\nDone Successfully. New balance is: " << CurrentClient.AccountBalance << "\n\n\n";
			}

		}
	}

	void ShowQucikWithdrawScreen()
	{
		vector<sClient> vClient = LoadClientsDataFromFile(FileName);


		cout << "=============================================\n";
		cout << "             Qucik Withdraw Screen \n";
		cout << "=============================================\n";
		cout << "  \t [1] 20";
		cout << "\t\t         [2] 50\n";
		cout << "  \t [3] 100";
		cout << "\t\t [4] 200\n";
		cout << "  \t [5] 400";
		cout << "\t\t [6] 600\n";
		cout << "  \t [7] 800";
		cout << "\t\t [8] 1000\n";
		cout << "  \t [9] Exit\n\n\n";
		cout << "=============================================\n";

		cout << "Your Balance is " << CurrentClient.AccountBalance << "\n";

		int ChoiseNumber = ReadNumberPositive("Choose what to withdraw from[1] to [8] ? ");

		PerformQucikWithdrawOption(ChoiseNumber, vClient);
	}

	void PerformATMMainMenuOption(int ChoiseNumber, vector<sClient>& vClient)
	{
		switch (ChoiseNumber)
		{
		case enAtmSystem::QuickWithdarw:
		case enAtmSystem::NormalWithdraw:
		case enAtmSystem::Deposit:
		case enAtmSystem::CheckBalance:
		{
			system("cls");
			ExecuteATMMenuOption(ChoiseNumber, vClient);
			system("pause");
			break;
		}

		case enAtmSystem::Logout:
		{
			system("cls");
			Login();
			break;
		}
		}
	}

	void ShowATMMainMenu()
	{
		int ChoiseNumber;

		do
		{
			system("cls");


			vector<sClient> vClient = LoadClientsDataFromFile(FileName);


			cout << "=============================================\n";
			cout << "                 ATM Main Menu Screen\n";
			cout << "=============================================\n";
			cout << "           [1] Quick Withdraw.\n";
			cout << "           [2] Normal Withdraw.\n";
			cout << "           [3] Deposit.\n";
			cout << "           [4] Check Balance.\n";
			cout << "           [5] Logout.\n";
			cout << "=============================================\n";

			 ChoiseNumber = ReadNumberPositive("Choose what do you Wand to do? [1 to 5]? ");

			PerformATMMainMenuOption(ChoiseNumber, vClient);

		} while (ChoiseNumber != enAtmSystem::Logout);
	
	}

	void Login()
	{
		sClient Client;

		bool Answer;


		vector<sClient> vClient = LoadClientsDataFromFile(FileName);

		ShowLoginScreen();

		do {
			Client.AccountNumber = ReadString("Enter AccountNumber? ");
			Client.PinCode = ReadString("Enter PinCode? ");


			if (IsClientExistAccountNumberAndBinCode(vClient, Client.AccountNumber, Client.PinCode))
			{

				CurrentClient = ResearchClintByAccountNumber(vClient, Client.AccountNumber);

				Answer = true;
				system("cls");
			}
			else
			{
				system("cls");
				ShowLoginScreen();
				cout << "Invalid Account Number/PinCode!\n";
				Answer = false;
			}
		} while (Answer == false);

		ShowATMMainMenu();
	}


}

























