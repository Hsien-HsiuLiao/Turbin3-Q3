#include "CustomSolanaUtils.h"
#include "SolanaUtils.h"


// Custom sendRawTransaction with enhanced JSON-RPC parameters
bool sendRawTransactionCustom(const String &txBase64, String &outSignature) {
    // Custom implementation with your desired JSON-RPC parameters
    WiFiClientSecure client;
    client.setInsecure(); // Skip certificate validation (for ESP32)
    HTTPClient http;

    // Use the RPC_URL from your constants
    if (!http.begin(client, solanaRpcUrl)) {
        Serial.println("HTTP begin failed for sendRawTransaction");
        return false;
    }

    // Set request headers for JSON-RPC
    http.addHeader("Content-Type", "application/json");

    // Your custom JSON-RPC body with the parameters you want
    String body = String() +
        R"({"jsonrpc":"2.0","id":1,"method":"sendTransaction","params":[")" +
        txBase64 + 
        R"(",{"encoding":"base64","commitment":"confirmed","sigVerify":false,"replaceRecentBlockhash":true,"accounts":{"encoding":"base64","addresses":[]},"logs":true}]})";

    // Execute HTTP POST request
    int code = http.POST(body);
    if (code != 200) {
        Serial.printf("sendTransaction HTTP code: %d\n", code);
        String resp = http.getString();
        Serial.println("Response: " + resp);
        http.end();
        return false;
    }

    // Parse response and deserialize JSON
    String response = http.getString();
    http.end();

    DynamicJsonDocument doc(2048);
    auto err = deserializeJson(doc, response);
    if (err) {
        Serial.println("JSON parse error in sendTransaction");
        return false;
    }

    // Check for error field in the RPC response
    if (doc["error"]) {
        Serial.println("\n=== ❌ RPC Error Details ===");
        Serial.println("Error Code: " + String(doc["error"]["code"].as<int>()));
        Serial.println("Error Message: " + doc["error"]["message"].as<String>());
        
        // Check for detailed error data
        if (doc["error"].containsKey("data")) {
            Serial.println("Error Data: " + doc["error"]["data"].as<String>());
        }
        
        // Check for logs in error response
        if (doc["error"].containsKey("logs")) {
            Serial.println("\n=== 📋 Error Logs ===");
            JsonArray errorLogs = doc["error"]["logs"];
            for (JsonVariant log : errorLogs) {
                Serial.println("📝 " + log.as<String>());
            }
            Serial.println("=== End Error Logs ===\n");
        }
        
        Serial.println("=== End RPC Error Details ===\n");
        return false;
    }

    // Extract transaction signature from result
    outSignature = doc["result"].as<String>();
    
    // Parse and display transaction logs if available
    if (doc["result"].is<JsonObject>()) {
        JsonObject result = doc["result"];
        if (result.containsKey("logs")) {
            Serial.println("\n=== 📋 Transaction Logs ===");
            JsonArray logs = result["logs"];
            for (JsonVariant log : logs) {
                Serial.println("📝 " + log.as<String>());
            }
            Serial.println("=== End Logs ===\n");
        }
    }
    
    return true;
}

// Function to get transaction logs by signature
bool getTransactionLogs(const String &signature) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;

    if (!http.begin(client, solanaRpcUrl)) {
        Serial.println("HTTP begin failed for getTransaction");
        return false;
    }

    http.addHeader("Content-Type", "application/json");

    // Request transaction details with logs
    String body = String() +
        R"({"jsonrpc":"2.0","id":1,"method":"getTransaction","params":[")" +
        signature + 
        R"(",{"encoding":"json","commitment":"confirmed","maxSupportedTransactionVersion":0}]})";

    int code = http.POST(body);
    if (code != 200) {
        Serial.printf("getTransaction HTTP code: %d\n", code);
        String resp = http.getString();
        Serial.println("Response: " + resp);
        http.end();
        return false;
    }

    String response = http.getString();
    http.end();

    DynamicJsonDocument doc(4096); // Larger buffer for transaction data
    auto err = deserializeJson(doc, response);
    if (err) {
        Serial.println("JSON parse error in getTransaction");
        return false;
    }

    if (doc["error"]) {
        Serial.println("RPC Error: " + doc["error"]["message"].as<String>());
        return false;
    }

    JsonObject result = doc["result"];
    if (result.isNull()) {
        Serial.println("❌ Transaction not found or not confirmed yet");
        return false;
    }

    // Display transaction details
    Serial.println("\n=== 📊 Transaction Details ===");
    Serial.println("Signature: " + signature);
    
    if (result.containsKey("slot")) {
        Serial.println("Slot: " + String(result["slot"].as<uint64_t>()));
    }
    
    if (result.containsKey("blockTime")) {
        uint64_t blockTime = result["blockTime"].as<uint64_t>();
        Serial.println("Block Time: " + String(blockTime));
    }

    // Display logs
    if (result.containsKey("meta") && result["meta"].containsKey("logMessages")) {
        Serial.println("\n=== 📋 Program Logs ===");
        JsonArray logs = result["meta"]["logMessages"];
        for (JsonVariant log : logs) {
            Serial.println("📝 " + log.as<String>());
        }
        Serial.println("=== End Program Logs ===\n");
    }

    // Display compute units consumed
    if (result.containsKey("meta") && result["meta"].containsKey("computeUnitsConsumed")) {
        Serial.println("Compute Units Consumed: " + String(result["meta"]["computeUnitsConsumed"].as<uint64_t>()));
    }

    // Display fee
    if (result.containsKey("meta") && result["meta"].containsKey("fee")) {
        Serial.println("Transaction Fee: " + String(result["meta"]["fee"].as<uint64_t>()) + " lamports");
    }

    // Display error if any
    if (result.containsKey("meta") && result["meta"].containsKey("err")) {
        Serial.println("\n=== ❌ Transaction Error Analysis ===");
        JsonObject error = result["meta"]["err"];
        
        // Print the raw error
        Serial.println("Raw Error: " + result["meta"]["err"].as<String>());
        
        // If it's a custom program error, decode it
        if (error.containsKey("Custom")) {
            int errorCode = error["Custom"];
            Serial.println("Custom Program Error Code: 0x" + String(errorCode, HEX));
            Serial.println("Error Code (decimal): " + String(errorCode));
            
            // Common Solana program error codes
            switch (errorCode) {
                case 0x7d6:
                    Serial.println("❌ Error 0x7d6: Likely an account validation or constraint violation");
                    Serial.println("   Common causes:");
                    Serial.println("   - Account not found or invalid");
                    Serial.println("   - Insufficient permissions");
                    Serial.println("   - Invalid account state");
                    Serial.println("   - PDA derivation mismatch");
                    break;
                case 0x0:
                    Serial.println("❌ Error 0x0: Generic program error");
                    break;
                case 0x1:
                    Serial.println("❌ Error 0x1: Invalid instruction");
                    break;
                case 0x2:
                    Serial.println("❌ Error 0x2: Invalid account data");
                    break;
                case 0x3:
                    Serial.println("❌ Error 0x3: Invalid account owner");
                    break;
                default:
                    Serial.println("❌ Unknown custom program error");
                    break;
            }
        }
        
        // Check for instruction errors
        if (error.containsKey("InstructionError")) {
            JsonArray instructionError = error["InstructionError"];
            Serial.println("Instruction Error at index: " + String(instructionError[0].as<int>()));
            Serial.println("Error: " + instructionError[1].as<String>());
        }
        
        Serial.println("=== End Error Analysis ===\n");
    } else {
        Serial.println("✅ Transaction Successful");
    }

    return true;
}

// Function to simulate transaction and get detailed logs
bool simulateTransaction(const String &txBase64) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;

    if (!http.begin(client, solanaRpcUrl)) {
        Serial.println("HTTP begin failed for simulateTransaction");
        return false;
    }

    http.addHeader("Content-Type", "application/json");

    // Simulate transaction with detailed logging
    String body = String() +
        R"({"jsonrpc":"2.0","id":1,"method":"simulateTransaction","params":[")" +
        txBase64 + 
        R"(",{"encoding":"base64","commitment":"confirmed","sigVerify":false,"replaceRecentBlockhash":true,"accounts":{"encoding":"base64","addresses":[]},"logs":true}]})";

    int code = http.POST(body);
    if (code != 200) {
        Serial.printf("simulateTransaction HTTP code: %d\n", code);
        String resp = http.getString();
        Serial.println("Response: " + resp);
        http.end();
        return false;
    }

    String response = http.getString();
    http.end();

    DynamicJsonDocument doc(4096);
    auto err = deserializeJson(doc, response);
    if (err) {
        Serial.println("JSON parse error in simulateTransaction");
        return false;
    }

    if (doc["error"]) {
        Serial.println("\n=== ❌ Simulation Error ===");
        Serial.println("Error: " + doc["error"]["message"].as<String>());
        return false;
    }

    JsonObject result = doc["result"];
    if (result.isNull()) {
        Serial.println("❌ Simulation result is null");
        return false;
    }

    Serial.println("\n=== 🔍 Transaction Simulation Results ===");
    
    // Check for simulation errors
    if (result.containsKey("err")) {
        Serial.println("❌ Simulation Failed");
        Serial.println("Error: " + result["err"].as<String>());
        
        // Detailed error analysis
        JsonObject error = result["err"];
        if (error.containsKey("Custom")) {
            int errorCode = error["Custom"];
            Serial.println("Custom Program Error: 0x" + String(errorCode, HEX));
        }
    } else {
        Serial.println("✅ Simulation Successful");
    }

    // Display logs
    if (result.containsKey("logs")) {
        Serial.println("\n=== 📋 Simulation Logs ===");
        JsonArray logs = result["logs"];
        for (JsonVariant log : logs) {
            Serial.println("📝 " + log.as<String>());
        }
        Serial.println("=== End Simulation Logs ===\n");
    }

    // Display compute units
    if (result.containsKey("unitsConsumed")) {
        Serial.println("Compute Units Consumed: " + String(result["unitsConsumed"].as<uint64_t>()));
    }

    return true;
}
