#ifndef CUSTOM_SOLANA_UTILS_H
#define CUSTOM_SOLANA_UTILS_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Custom sendRawTransaction with enhanced JSON-RPC parameters
bool sendRawTransactionCustom(const String &txBase64, String &outSignature);

// Function to get transaction logs by signature
bool getTransactionLogs(const String &signature);

// Function to simulate transaction and get detailed logs
bool simulateTransaction(const String &txBase64);

#endif // CUSTOM_SOLANA_UTILS_H
