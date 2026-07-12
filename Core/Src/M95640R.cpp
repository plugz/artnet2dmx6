/**
 ******************************************************************************
 * @file    S2LP.cpp
 * @author  SRA
 * @version V1.0.0
 * @date    March 2020
 * @brief   Implementation of a S2-LP sub-1GHz transceiver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2020 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/

#include "M95640R.hpp"

/* Defines -------------------------------------------------------------------*/

/* Class Implementation ------------------------------------------------------*/

/** Constructor
 * @param spi object of the instance of the spi peripheral
 * @param ns the spi chip select pin
 */
M95640R::M95640R()
{
}

void M95640R::setup(GPIO_TypeDef* csPinPeripheral, uint16_t csPin, SPI_HandleTypeDef* spiHandle,
                    GPIO_TypeDef* writeGpioPeripheral, uint16_t writeGpio,
                    GPIO_TypeDef* holdGpioPeripheral, uint16_t holdGpio) {
    _csPinPeripheral = csPinPeripheral;
    _csPin = csPin;
    _spiHandle = spiHandle;

    _writeGpioPeripheral = writeGpioPeripheral;
    _writeGpio = writeGpio;
    _holdGpioPeripheral = holdGpioPeripheral;
    _holdGpio = holdGpio;
}

/**
* @brief  Initialize the M95640R library.
* @param  None.
* @retval None.
*/
void M95640R::begin(void)
{
    HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(_writeGpioPeripheral, _writeGpio, GPIO_PIN_SET);
    HAL_GPIO_WritePin(_holdGpioPeripheral, _holdGpio, GPIO_PIN_SET);
}

/**
* @brief  DeInitialize the M95640R library.
* @param  None.
* @retval None.
*/
void M95640R::end(void)
{
}

/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  nAddress the address of the page
* @param  cNbBytes the size in bytes of the data to be read
* @param  pcBuffer pointer to store the read data
* @retval None
*/
void M95640R::EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd[3];
  cmd[0] = EEPROM_CMD_READ;

  for(uint8_t k=0; k<2; k++) {
    cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
  }

  /* Wait the end of a previous write operation */
  EepromWaitEndWriteOperation();

  /* Put the SPI chip select low to start the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);

  /* Write the header bytes and read the status bytes */
  HAL_SPI_TransmitReceive(_spiHandle, cmd, cmd, sizeof(cmd), HAL_MAX_DELAY);

  /* Read the registers according to the number of bytes */
  HAL_SPI_Receive(_spiHandle, pcBuffer, cNbBytes, HAL_MAX_DELAY);

  /* Put the SPI chip select high to end the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);

  return;
}

/**
* @brief  Write a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
*         It is allowed to write only a page for each operation. If the bytes
*         exceed the single page location, the other bytes are written at the
*         beginning.
* @param  nAddress the address of the page
* @param  cNbBytes the size in bytes of the data to be written
* @param  pcBuffer pointer to the data to be written
* @retval None
*/
void M95640R::EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd = EEPROM_CMD_WRITE;
  uint8_t address[2];

  /* Wait the end of a previous write operation */
  EepromWaitEndWriteOperation();

  /* SET the WREN flag */
  EepromWriteEnable();

  for(uint8_t k=0; k<2; k++) {
    address[k] = (uint8_t)(nAddress>>((1-k)*8));
  }

  /* Put the SPI chip select low to start the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);

  /* Write the header bytes and read the SPIRIT status bytes */
  HAL_SPI_TransmitReceive(_spiHandle, &cmd, &cmd, sizeof(cmd), HAL_MAX_DELAY);

  HAL_SPI_TransmitReceive(_spiHandle, address, address, sizeof(address), HAL_MAX_DELAY);

  HAL_SPI_Transmit(_spiHandle, pcBuffer, cNbBytes, HAL_MAX_DELAY);

  /* Put the SPI chip select high to end the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);

  return;
}

/**
* @brief  Wait polling the SPI until the internal WIP flag is RESET.
*         The flag is SET when a write operation is running.
* @param  None
* @retval None
*/
void M95640R::EepromWaitEndWriteOperation(void)
{
  uint8_t cmd = EEPROM_CMD_RDSR;
  uint8_t status;

  /* Put the SPI chip select low to start the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);

  /* Send command */
  HAL_SPI_TransmitReceive(_spiHandle, &cmd, &cmd, sizeof(cmd), HAL_MAX_DELAY);

  /* Polling on status register */
  do{
    HAL_SPI_Receive(_spiHandle, &status, sizeof(status), HAL_MAX_DELAY);
  }while(status&EEPROM_STATUS_WIP);

  /* Put the SPI chip select high to end the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);

  return;
}

/**
* @brief EepromWriteEnable
* @param  None
* @retval None
*/
void M95640R::EepromWriteEnable(void)
{
  uint8_t cmd = EEPROM_CMD_WREN;

  /* Put the SPI chip select low to start the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);

  /* Send command */
  HAL_SPI_TransmitReceive(_spiHandle, &cmd, &cmd, sizeof(cmd), HAL_MAX_DELAY);

  /* Put the SPI chip select high to end the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);

  return;
}

/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t M95640R::EepromStatus(void)
{
  uint8_t cmd[2] = {EEPROM_CMD_RDSR, 0xFF};

  /* Put the SPI chip select low to start the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);

  /* Send command */
  HAL_SPI_TransmitReceive(_spiHandle, cmd, cmd, sizeof(cmd), HAL_MAX_DELAY);

  /* Put the SPI chip select high to end the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);

  return cmd[1];
}

/**
* @brief  Set the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t M95640R::EepromSetSrwd(void)
{
  uint8_t cmd[2] = {EEPROM_CMD_WRSR, EEPROM_STATUS_SRWD};

  /* Put the SPI chip select low to start the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);

  /* Send command */
  HAL_SPI_TransmitReceive(_spiHandle, cmd, cmd, sizeof(cmd), HAL_MAX_DELAY);

  /* Put the SPI chip select high to end the transaction */
  HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);

  return cmd[1];
}
