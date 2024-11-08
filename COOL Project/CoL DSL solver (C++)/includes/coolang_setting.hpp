/*
 * coolang_setting.hpp
 *
 *  Created on: 2022年8月12日
 *      Author: -
 */

#ifndef COOLANG_SETTING_HPP_
#define COOLANG_SETTING_HPP_

#include "coolang_define.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <deque>
#include <map>
#include <cstdio>
#include <filesystem>
#include <io.h>
#ifndef Intg
#define Intg int
#endif
#ifndef Intl
#define Intl int64_t
#endif
#ifndef Numb
#define Numb double
#endif

namespace COOLANG {
	using namespace std;
	class SettingBinaryInfo;

	class Setting {
	public:
		char version[64 * sizeof(char)] = "COOLANG2022"; //起始标识
		Intg codeForm = 0; //0 不生成代码,1   生成cos代码，2   生成coc代码，3   生成cob代码
		bool multiFile = false;

		Intg matchStateSiloMaxCapacity = 100;
		Intg groundingStateSiloMaxCapacity = 1000;
		Intg maxSwitchTurn = 50;
		Intg maxBackStep = 50;

		bool debugVersion = false;
		bool execute = true;

		bool debugMode = false;
		char debugInfoOutputPath[PATH_MAX * sizeof(char)];
		char debugOrderInputPath[PATH_MAX * sizeof(char)];

		Intg baseAlgorithm = 0;
		bool userPromptEnabled = 1;
		bool chainEnabled = 1;
		Intg userPromptAlgorithm = 0;
		Numb progressRewardFactor = 0.2;

		bool neuralNetworkEnabled = 0;
		Intg neuralNetworkAlgorithm = 0;
		bool train = 0;
		bool nnPenaltyEnabled = 1;
		bool nnRewardEnabled = 1;
		bool exportModelingData = 1;
		bool exportSuccessTrainingData = 1;
		bool exportFailureTrainingData = 1;
		bool exportStepSkippingTrainingData = 1;
		bool exportDuplicatedTrainingData = 0;
		bool exportPerformance = 1;
		Numb cosineSimilarityThreshold = 0.8;
		Numb sampleProportion = 0.5;

		//BDDB coefficients
		Numb BDDB__r_a__k_0 = 1;
		Numb BDDB__r_a_base = 1;
		Numb BDDB__o__k_0 = 5;
		Numb BDDB__o__k_1 = 0.1;
		Numb BDDB__o__k_2 = 1.1;
		Numb BDDB_lambda = 0.95;
		Numb BDDB__q_base = 10;

		Intg CollectionCycleSerialNumber = -1;
		Setting() {
		}
		bool loadSettingFile(const pathStr& fileName) {
			fstream fs;
			std::map<string, Intg > option_value_map;

			try {

				fs.open(std::filesystem::path(fileName), fs.in);
			}
			catch (exception& e) {
				cout << "setting file open err,exit(-1):" << e.what() << endl;
				exit(-1);

			}
			string line;
			while (getline(fs, line)) {
				std::vector<string>&& vs = split(line, ":");
				if (vs.size() < 2) {
					continue;
				}
				else {

					if (vs[0] == "version") {
						strcpy_s(this->version, vs[1].c_str());

					}
					else if (vs[0] == "codeForm") {
						this->codeForm = toIntg(vs[1]);

					}
					else if (vs[0] == "multiFile") {
						this->multiFile = toIntg(vs[1]);
					}
					else if (vs[0] == "matchStateSiloMaxCapacity") {
						this->matchStateSiloMaxCapacity = toIntg(vs[1]);
					}
					else if (vs[0] == "groundingStateSiloMaxCapacity") {
						this->groundingStateSiloMaxCapacity = toIntg(vs[1]);
					}
					else if (vs[0] == "maxSwitchTurn") {
						this->maxSwitchTurn = toIntg(vs[1]);
					}
					else if (vs[0] == "maxBackStep") {
						this->maxBackStep = toIntg(vs[1]);
					}
					else if (vs[0] == "debugVersion") {
						this->debugVersion = toIntg(vs[1]);
					}
					else if (vs[0] == "execute") {
						this->execute = toIntg(vs[1]);
					}
					else if (vs[0] == "debugMode") {
						this->debugMode = toIntg(vs[1]);
					}
					else if (vs[0] == "debugInfoOutputPath") {
						strcpy_s(this->debugInfoOutputPath, vs[1].c_str());

					}

					else if (vs[0] == "baseAlgorithm") {
						this->baseAlgorithm = toIntg(vs[1]);
					}
					else if (vs[0] == "userPromptEnabled") {
						this->userPromptEnabled = toIntg(vs[1]);
					}
					else if (vs[0] == "chainEnabled") {
						this->chainEnabled = toIntg(vs[1]);
					}
					else if (vs[0] == "userPromptAlgorithm") {
						this->userPromptAlgorithm = toIntg(vs[1]);
					}
					else if (vs[0] == "progressRewardFactor") {
						this->progressRewardFactor = toNumb(vs[1]);
					}
					else if (vs[0] == "neuralNetworkEnabled") {
						this->neuralNetworkEnabled = toIntg(vs[1]);
					}
					else if (vs[0] == "neuralNetworkAlgorithm") {
						this->neuralNetworkAlgorithm = toIntg(vs[1]);
					}
					else if (vs[0] == "train") {
						this->train = toIntg(vs[1]);
					}
					else if (vs[0] == "nnPenaltyEnabled") {
						this->nnPenaltyEnabled = toIntg(vs[1]);
					}
					else if (vs[0] == "nnRewardEnabled") {
						this->nnRewardEnabled = toIntg(vs[1]);
					}
					else if (vs[0] == "exportModelingData") {
						this->exportModelingData = toIntg(vs[1]);
					}
					else if (vs[0] == "exportSuccessTrainingData") {
						this->exportSuccessTrainingData = toIntg(vs[1]);
					}
					else if (vs[0] == "exportFailureTrainingData") {
						this->exportFailureTrainingData = toIntg(vs[1]);
					}
					else if (vs[0] == "exportStepSkippingTrainingData") {
						this->exportStepSkippingTrainingData = toIntg(vs[1]);
					}
					else if (vs[0] == "exportDuplicatedTrainingData") {
						this->exportDuplicatedTrainingData = toIntg(vs[1]);
					}
					else if (vs[0] == "exportPerformance") {
						this->exportPerformance = toIntg(vs[1]);
					}
					else if (vs[0] == "cosineSimilarityThreshold") {
						this->cosineSimilarityThreshold = toNumb(vs[1]);
					}
					else if (vs[0] == "sampleProportion") {
						this->sampleProportion = toNumb(vs[1]);
					}


					else if (vs[0] == "BDDB__r_a__k_0") {
						this->BDDB__r_a__k_0 = toNumb(vs[1]);
					}
					else if (vs[0] == "BDDB__r_a_base") {
						this->BDDB__r_a_base = toNumb(vs[1]);
					}
					else if (vs[0] == "BDDB__o__k_0") {
						this->BDDB__o__k_0 = toNumb(vs[1]);
					}
					else if (vs[0] == "BDDB__o__k_1") {
						this->BDDB__o__k_1 = toNumb(vs[1]);
					}
					else if (vs[0] == "BDDB__o__k_2") {
						this->BDDB__o__k_2 = toNumb(vs[1]);
					}
					else if (vs[0] == "BDDB_lambda") {
						this->BDDB_lambda = toNumb(vs[1]);
					}
					else if (vs[0] == "BDDB__q_base") {
						this->BDDB__q_base = toNumb(vs[1]);
					}

					else if (vs[0] == "CollectionCycleSerialNumber") {
						this->CollectionCycleSerialNumber = toIntg(vs[1]);
					}

				}

			}

			return true;
		}
		std::vector<string> split(const string& strg, const string& pattern) {
			std::vector<string> result;
			string str = strg;
			str += pattern;
			for (int i = 0; i < (int)str.size(); i++) {
				int pos = (int)str.find(pattern, i);
				if (pos != (int)string::npos && str.substr(i, pos - i) != "") {
					result.push_back(str.substr(i, pos - i));
					i = pos + (int)pattern.size() - 1;
				}
			}
			return result;
		}

		Intg toIntg(string s) {
			if (s == "") {
				{
#if debug
					static int ignorecount = 0;
					cerr << "toIntg(string)" << " convert \"\" to Intg,return 0"
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< "vm.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
				}
			}
			return atoi(s.c_str());
		}

		Numb toNumb(const std::string& s) {
			if (s == "") {
				{
#if debug
					static int ignorecount = 0;
					cerr << "toNumb(Strg)" << " convert \"\" to Numb,return 0"
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
#endif
				}
				return 0;
			}
			stringstream ss;
			ss.precision(PRECISION);
			ss << s;
			Numb num;
			ss >> num;
			if (num < 1e-50 && num > -1e-50) {
				num = 0;
			}
			{
#if debug
				static int ignorecount = 0;
				cout << "toNumb(Strg)" << "s:[" << s << "]\tnum:[" << num << "]"
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
#endif
			}
			return num;
		}

	};
	class SettingBinaryInfo {
	public:
		char version[64 * sizeof(char)]; //起始标识
		Intg codeForm = 0; //0,1   生成cos代码，2   生成coc代码，3   生成cob代码
		bool multiFile = false;

		Intg matchStateSiloMaxCapacity = 5;
		Intg groundingStateSiloMaxCapacity = 10;
		Intg maxSwitchTurn = 15;
		Intg maxBackStep = 10;

		bool debugVersion = false;
		bool execute = false;

		bool debugMode = false;
		char debugInfoOutputPath[PATH_MAX * sizeof(char)];
		char debugOrderInputPath[PATH_MAX * sizeof(char)];

		Intg baseAlgorithm = 0;
		bool userPromptEnabled = 1;
		bool chainEnabled = 1;
		Intg userPromptAlgorithm = 0;
		Numb progressRewardFactor = 0.2;
		bool neuralNetworkEnabled = 1;
		Intg neuralNetworkAlgorithm = 0;
		bool train = 1;
		bool nnPenaltyEnabled = 1;
		bool nnRewardEnabled = 1;
		bool exportModelingData = 1;
		bool exportSuccessTrainingData = 1;
		bool exportFailureTrainingData = 1;
		bool exportStepSkippingTrainingData = 1;
		bool exportDuplicatedTrainingData = 0;
		bool exportPerformance = 1;
		Numb cosineSimilarityThreshold = 0.8;
		Numb sampleProportion = 0.5;


		//BDDB coefficients
		Numb BDDB__r_a__k_0 = 1;
		Numb BDDB__r_a_base = 1;
		Numb BDDB__o__k_0 = 5;
		Numb BDDB__o__k_1 = 0.1;
		Numb BDDB__o__k_2 = 1.1;
		Numb BDDB_lambda = 0.95;
		Numb BDDB__q_base = 10;

		Intg CollectionCycleSerialNumber = -1;

		void parseObject(const Setting& setting) {
			strcpy_s(this->version, setting.version);
			this->codeForm = setting.codeForm;
			this->multiFile = setting.multiFile;

			this->matchStateSiloMaxCapacity = setting.matchStateSiloMaxCapacity;
			this->groundingStateSiloMaxCapacity =
				setting.groundingStateSiloMaxCapacity;
			this->maxSwitchTurn = setting.maxSwitchTurn;
			this->maxBackStep = setting.maxBackStep;

			this->debugVersion = setting.debugVersion;
			this->execute = setting.execute;

			this->debugMode = setting.debugMode;
			strcpy_s(this->debugInfoOutputPath, setting.debugInfoOutputPath);
			strcpy_s(this->debugOrderInputPath, setting.debugOrderInputPath);

			this->baseAlgorithm = setting.baseAlgorithm;
			this->userPromptEnabled = setting.userPromptEnabled;
			this->chainEnabled = setting.chainEnabled;
			this->userPromptAlgorithm = setting.userPromptAlgorithm;
			this->progressRewardFactor = setting.progressRewardFactor;
			this->neuralNetworkEnabled = setting.neuralNetworkEnabled;
			this->neuralNetworkAlgorithm = setting.neuralNetworkAlgorithm;
			this->train = setting.train;
			this->nnPenaltyEnabled = setting.nnPenaltyEnabled;
			this->nnRewardEnabled = setting.nnRewardEnabled;
			this->exportModelingData = setting.exportModelingData;
			this->exportSuccessTrainingData =
				setting.exportSuccessTrainingData;
			this->exportFailureTrainingData =
				setting.exportFailureTrainingData;
			this->exportStepSkippingTrainingData =
				setting.exportStepSkippingTrainingData;
			this->exportDuplicatedTrainingData =
				setting.exportDuplicatedTrainingData;
			this->exportPerformance = setting.exportPerformance;
			this->cosineSimilarityThreshold = setting.cosineSimilarityThreshold;
			this->sampleProportion = setting.sampleProportion;

			this->BDDB__r_a__k_0 = setting.BDDB__r_a__k_0;
			this->BDDB__r_a_base = setting.BDDB__r_a_base;
			this->BDDB__o__k_0 = setting.BDDB__o__k_0;
			this->BDDB__o__k_1 = setting.BDDB__o__k_1;
			this->BDDB__o__k_2 = setting.BDDB__o__k_2;
			this->BDDB_lambda = setting.BDDB_lambda;
			this->BDDB__q_base = setting.BDDB__q_base;

			this->CollectionCycleSerialNumber = setting.CollectionCycleSerialNumber;

		}
		Intl parseAndWriteObject(const Setting& setting, fstream& fs) {

			strcpy_s(this->version, setting.version);
			this->codeForm = setting.codeForm;
			this->multiFile = setting.multiFile;

			this->matchStateSiloMaxCapacity = setting.matchStateSiloMaxCapacity;
			this->groundingStateSiloMaxCapacity =
				setting.groundingStateSiloMaxCapacity;
			this->maxSwitchTurn = setting.maxSwitchTurn;
			this->maxBackStep = setting.maxBackStep;

			this->debugVersion = setting.debugVersion;
			this->execute = setting.execute;

			this->debugMode = setting.debugMode;
			strcpy_s(this->debugInfoOutputPath, setting.debugInfoOutputPath);
			strcpy_s(this->debugOrderInputPath, setting.debugOrderInputPath);

			this->baseAlgorithm = setting.baseAlgorithm;
			this->userPromptEnabled = setting.userPromptEnabled;
			this->chainEnabled = setting.chainEnabled;
			this->userPromptAlgorithm = setting.userPromptAlgorithm;
			this->progressRewardFactor = setting.progressRewardFactor;
			this->neuralNetworkEnabled = setting.neuralNetworkEnabled;
			this->neuralNetworkAlgorithm = setting.neuralNetworkAlgorithm;
			this->train = setting.train;
			this->nnPenaltyEnabled = setting.nnPenaltyEnabled;
			this->nnRewardEnabled = setting.nnRewardEnabled;
			this->exportModelingData = setting.exportModelingData;
			this->exportSuccessTrainingData =
				setting.exportSuccessTrainingData;
			this->exportFailureTrainingData =
				setting.exportFailureTrainingData;
			this->exportStepSkippingTrainingData =
				setting.exportStepSkippingTrainingData;
			this->exportDuplicatedTrainingData =
				setting.exportDuplicatedTrainingData;
			this->exportPerformance = setting.exportPerformance;
			this->cosineSimilarityThreshold = setting.cosineSimilarityThreshold;
			this->sampleProportion = setting.sampleProportion;


			this->BDDB__r_a__k_0 = setting.BDDB__r_a__k_0;
			this->BDDB__r_a_base = setting.BDDB__r_a_base;
			this->BDDB__o__k_0 = setting.BDDB__o__k_0;
			this->BDDB__o__k_1 = setting.BDDB__o__k_1;
			this->BDDB__o__k_2 = setting.BDDB__o__k_2;
			this->BDDB_lambda = setting.BDDB_lambda;
			this->BDDB__q_base = setting.BDDB__q_base;

			this->CollectionCycleSerialNumber = setting.CollectionCycleSerialNumber;





			return fs.tellp();    //info及其子结构中无指针结构，上层结构中会将其写入fs，故不再重复写入。此处仅解析

		}
		Intl constructObject(Setting& setting, fstream& fs) {

			strcpy_s(setting.version, this->version);
			setting.matchStateSiloMaxCapacity = this->matchStateSiloMaxCapacity;
			setting.groundingStateSiloMaxCapacity =
				this->groundingStateSiloMaxCapacity;
			setting.maxSwitchTurn = this->maxSwitchTurn;

			strcpy_s(setting.version, this->version);
			setting.codeForm = this->codeForm;
			setting.multiFile = this->multiFile;

			setting.matchStateSiloMaxCapacity = this->matchStateSiloMaxCapacity;
			setting.groundingStateSiloMaxCapacity =
				this->groundingStateSiloMaxCapacity;
			setting.maxSwitchTurn = this->maxSwitchTurn;
			setting.maxBackStep = this->maxBackStep;

			setting.debugVersion = this->debugVersion;
			setting.execute = this->execute;

			setting.debugMode = this->debugMode;
			strcpy_s(setting.debugInfoOutputPath, this->debugInfoOutputPath);
			strcpy_s(setting.debugOrderInputPath, this->debugOrderInputPath);

			setting.baseAlgorithm = this->baseAlgorithm;
			setting.userPromptEnabled = this->userPromptEnabled;
			setting.chainEnabled = this->chainEnabled;
			setting.userPromptAlgorithm = this->userPromptAlgorithm;
			setting.progressRewardFactor = this->progressRewardFactor;
			setting.neuralNetworkEnabled = this->neuralNetworkEnabled;
			setting.neuralNetworkAlgorithm = this->neuralNetworkAlgorithm;
			setting.train = this->train;
			setting.nnPenaltyEnabled = this->nnPenaltyEnabled;
			setting.nnRewardEnabled = this->nnRewardEnabled;
			setting.exportModelingData = this->exportModelingData;
			setting.exportSuccessTrainingData =
				this->exportSuccessTrainingData;
			setting.exportFailureTrainingData =
				this->exportFailureTrainingData;
			setting.exportStepSkippingTrainingData =
				this->exportStepSkippingTrainingData;
			setting.exportDuplicatedTrainingData =
				this->exportDuplicatedTrainingData;
			setting.exportPerformance = this->exportPerformance;
			setting.cosineSimilarityThreshold = this->cosineSimilarityThreshold;
			setting.sampleProportion = this->sampleProportion;


			setting.BDDB__r_a__k_0 = this->BDDB__r_a__k_0;
			setting.BDDB__r_a_base = this->BDDB__r_a_base;
			setting.BDDB__o__k_0 = this->BDDB__o__k_0;
			setting.BDDB__o__k_1 = this->BDDB__o__k_1;
			setting.BDDB__o__k_2 = this->BDDB__o__k_2;
			setting.BDDB_lambda = this->BDDB_lambda;
			setting.BDDB__q_base = this->BDDB__q_base;


			setting.CollectionCycleSerialNumber = this->CollectionCycleSerialNumber;

			return fs.tellg();    //info及其子结构中无指针结构，上层结构中会将其写入fs，故不再重复写入。此处仅解析

		}

	};

}  // namespace COOLANG

#endif /* COOLANG_SETTING_HPP_ */
