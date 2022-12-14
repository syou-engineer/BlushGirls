#include "Direct3DData.h"

/**
* @brief D3DDeviceの設定
*/
VOID Direct3DData::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

/**
* @brief D3DDeviceを取得する
*/
LPDIRECT3DDEVICE9 Direct3DData::GetDevice()
{
	return m_pDevice;
}