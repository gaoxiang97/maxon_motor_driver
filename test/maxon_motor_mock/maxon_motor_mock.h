#pragma once

#include <gmock/gmock.h>
#include "ll_mock.h"

#include <memory>

struct MockMaxonMotorInterface {
public:
	virtual ~MockMaxonMotorInterface() {}

	MOCK_METHOD2(m_LL_TIM_OC_SetCompareCH1, void(TIM_TypeDef*, uint32_t));
	MOCK_METHOD2(m_LL_GPIO_ResetOutputPin, void(GPIO_TypeDef*, uint32_t));
	MOCK_METHOD2(m_LL_GPIO_SetOutputPin, void(GPIO_TypeDef*, uint32_t));

};

class MaxonMotorMock : public ::testing::Test {
public:
	MaxonMotorMock() {
		m_maxonMotorMock.reset(new ::testing::NiceMock<MockMaxonMotorInterface>());
	}

	~MaxonMotorMock() {
		m_maxonMotorMock.reset();
	}
public:
	static std::unique_ptr<MockMaxonMotorInterface> m_maxonMotorMock;
};

