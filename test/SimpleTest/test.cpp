#include "pch.h"

TEST(admf, SheenTest) {
	auto admfWrite = admf::createADMF();
	auto material = admfWrite->getMaterial();
	auto layer = material->getLayerArray()->append();
	auto basic = layer->getBasic();
	auto sheenColor = basic->getSheenColor();
	auto sheenGloss = basic->getSheenGloss();

	auto sheenColorRGB = sheenColor->getColor();
	float sheenColorR = 0.2f;
	float sheenColorG = 0.5f;
	float sheenColorB = 0.8f;
	sheenColorRGB->setR(sheenColorR);
	sheenColorRGB->setG(sheenColorG);
	sheenColorRGB->setB(sheenColorB);

	float sheenGlossF = 0.75f;
	sheenGloss->setValue(sheenGlossF);

	char path[] = R"(G://Sheen_Write.admf)";
	auto writeResult = admfWrite->saveToFile(path);
	EXPECT_EQ(writeResult, admf::ADMF_RESULT::ADMF_SUCCESS);

	admf::ADMF_RESULT readResult;
	auto admfRead = admf::loadFromFile(path, readResult);
	EXPECT_EQ(readResult, admf::ADMF_RESULT::ADMF_SUCCESS);
	auto readBasic = admfRead->getMaterial()->getLayerArray()->get(0)->getBasic();
	auto readSheenColor = readBasic->getSheenColor()->getColor();
	EXPECT_FLOAT_EQ(readSheenColor->getR(), sheenColorR);
	EXPECT_FLOAT_EQ(readSheenColor->getG(), sheenColorG);
	EXPECT_FLOAT_EQ(readSheenColor->getB(), sheenColorB);
	EXPECT_FLOAT_EQ(readBasic->getSheenGloss()->getValue(), sheenGlossF);
}