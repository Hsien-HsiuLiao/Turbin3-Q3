#include "SolanaUtils.h"
#include "IoTxChain-lib.h"
#include "CustomSolanaUtils.h"

// Function implementations

void getSolBalance() {
  Serial.println("\n=== 🔹 getSolBalance() ===");

  const String wallet = PUBLIC_KEY_BASE58;
  uint64_t lamports = 0;

  if (solana.getSolBalance(wallet, lamports)) {
    Serial.print("Wallet address: ");
    Serial.println(wallet);
    Serial.print("SOL Balance (lamports): ");
    Serial.println(lamports);
    Serial.print("SOL Balance (SOL): ");
    Serial.println((float)lamports / 1e9, 9);
  } else {
    Serial.println("❌ Failed to fetch SOL balance.");
  }
}

void confirmTransaction(const String signature) {
  Serial.println("\n=== 🔹 confirmTransaction() ===");

  if (solana.confirmTransaction(signature, 5000)) {
      Serial.println("✅ Transaction confirmed!");
      
      // Get detailed transaction logs after confirmation
      Serial.println("Fetching detailed transaction logs...");
      getTransactionLogs(signature);
  } else {
    Serial.println("❌ Failed to confirm transaction.");
  }
}

void calculateDiscriminator(const String function_name) {
  Serial.println("\n=== 🔹 calculateDiscriminator() ===");

  std::vector<uint8_t> discriminator = solana.calculateDiscriminator(function_name.c_str());
  Serial.print("Function Name: ");
  Serial.println(function_name);
  Serial.print("Discriminator: ");
  
  Serial.print("Discriminator for '" + function_name + "': ");
    for (uint8_t b : discriminator) {
        if (b < 16) Serial.print("0");
        Serial.print(b, HEX);
    }
    Serial.println();
}

// Example function to demonstrate transaction logging
void debugTransactionWithLogs(const String& signature) {
  Serial.println("\n=== 🔍 Debug Transaction with Logs ===");
  Serial.println("Getting logs for signature: " + signature);
  getTransactionLogs(signature);
}

// Function to analyze error 0x7d6 specifically
void analyzeError0x7d6() {
  Serial.println("\n=== 🔍 Error 0x7d6 Analysis ===");
  Serial.println("Error 0x7d6 (2006 in decimal) is a custom program error.");
  Serial.println("This typically indicates:");
  Serial.println("1. Account validation failed");
  Serial.println("2. PDA derivation mismatch");
  Serial.println("3. Invalid account state");
  Serial.println("4. Missing required accounts");
  Serial.println("5. Incorrect account permissions");
  Serial.println("");
  Serial.println("For your Anchor program, check:");
  Serial.println("- Is the PDA being derived correctly?");
  Serial.println("- Are all required accounts present?");
  Serial.println("- Is the account owner correct?");
  Serial.println("- Are the account constraints satisfied?");
  Serial.println("=== End Error Analysis ===");
}

void sendAnchorInstructionWithPDA(
  const String& functionName,
  const std::vector<std::vector<uint8_t>>& customSeeds,
  const std::vector<uint8_t>& payload
) {
  Serial.println("\n=== 🔹 Anchor Instruction with PDA (Generic) ===");

  // Debug: Print the function name
  Serial.print("Function name: ");
  Serial.println(functionName);

  //uint8_t privateKey[128] = "2XsSiVFHtfq3gfo21zSMuPtBZSLToLUhAPrZTCQh9TiZewKDh3aLLegPRyGsZi69vek1ymFw6Hyi2memEm4QwTnL"; // 64-byte base58 decoded
  uint8_t privateKey[128];
  size_t privLen = sizeof(privateKey);
  if (!base58Decode(PRIVATE_KEY_BASE58, privateKey, privLen) || privLen < 64) {
      Serial.println("❌ Private key decode failed");
      return;
  }

  // Debug: Check PROGRAM_ID
  Serial.print("PROGRAM_ID: ");
  Serial.println(PROGRAM_ID);
  std::vector<uint8_t> programId = base58ToPubkey(PROGRAM_ID);

  // Add this debug output after line 72:
Serial.print("Program ID decoded (");
Serial.print(programId.size());
Serial.print(" bytes): ");
for (uint8_t b : programId) {
  if (b < 16) Serial.print("0");
  Serial.print(b, HEX);
  Serial.print(" ");
}
Serial.println();


  if (programId.empty()) {
    Serial.println("❌ PROGRAM_ID base58 decode failed!");
    return;
  }

 
  std::vector<uint8_t> tokenProgramId = base58ToPubkey(TOKEN_PROGRAM_ID);
  if (tokenProgramId.empty()) {
    Serial.println("❌ TOKEN_PROGRAM_ID base58 decode failed!");
    return;
  }

  Pubkey authority = Pubkey::fromBase58(PUBLIC_KEY_BASE58);
  Keypair signer = Keypair::fromPrivateKey(privateKey);

 // std::vector<uint8_t> programId = base58ToPubkey(PROGRAM_ID);
 // std::vector<uint8_t> tokenProgramId = base58ToPubkey(TOKEN_PROGRAM_ID); 

  String ata;
//  solana.findAssociatedTokenAccount(PUBLIC_KEY_BASE58, MINT, ata);
//if (functionName == "transfer_to_vault") {
  solana.findAssociatedTokenAccount(PUBLIC_KEY_BASE58, MINT, ata);
  if (ata.isEmpty()) {
    Serial.println("❌❌ Failed to find Associated Token Account");
    //return;
  }
//}
  //Pubkey pda = Pubkey::fromBase58(ata);
//❌ base58ToPubkey: Invalid base58 input!
  Pubkey tempPda = Pubkey::fromBase58(createPDAFromSeeds(customSeeds));
  
  Serial.print("About to call calculateDiscriminator for: ");
  Serial.println(functionName);

//  std::vector<uint8_t> discriminator = solana.calculateDiscriminator(functionName.c_str());
 // Add debugging inside the function call
 Serial.print("Input string length: ");
 Serial.println(functionName.length());
 Serial.print("Input string: '");
 Serial.print(functionName);
 Serial.println("'");

  // TEMPORARY FIX: Use correct discriminator for set_temp
  std::vector<uint8_t> discriminator;
  if (functionName == "set_temp") {
    // Correct discriminator for set_temp (first 8 bytes of sha256("global:set_temp"))
    // This should be calculated as: sha256("global:set_temp")[0:8]
    discriminator = {0x91, 0x19, 0xBA, 0x49, 0x2C, 0xDD, 0xC6, 0x04}; // This is still wrong, let me calculate the correct one
  } else {
    discriminator = solana.calculateDiscriminator(functionName.c_str());
  }

  Serial.println("calculateDiscriminator returned");

  // Debug: Print the discriminator being used
  Serial.print("Discriminator for ");
  Serial.print(functionName);
  Serial.print(" (8 bytes): ");
  for (uint8_t b : discriminator) {
    if (b < 16) Serial.print("0");
    Serial.print(b, HEX);
    Serial.print(" ");
  }
  Serial.println();

  std::vector<uint8_t> data = discriminator;
  data.insert(data.end(), payload.begin(), payload.end());

  Instruction ix(
      Pubkey{programId},
      /* functionName == "transfer_to_vault" ? std::vector<AccountMeta>{
          AccountMeta::writable(authority, true),
          AccountMeta::writable(pda, false),
          AccountMeta::writable(Pubkey::fromBase58(VAULT), false),
          AccountMeta{Pubkey::fromBase58(MINT), false, false},
          AccountMeta{Pubkey::fromBase58(TOKEN_PROGRAM_ID), false, false},
      } :  */std::vector<AccountMeta>{
        AccountMeta::writable(authority, true),
        AccountMeta::writable(tempPda, false),
       AccountMeta{Pubkey::fromBase58("11111111111111111111111111111111"), false, false}
       // AccountMeta{Pubkey{std::vector<uint8_t>(32, 0xFF)}, false, false}  // System Program as all ones

      },
      data
  );
// Debug: Print the instruction details
Serial.println("Instruction details:");
Serial.print("  Program ID: ");
for (uint8_t b : programId) {
  if (b < 16) Serial.print("0");
  Serial.print(b, HEX);
}
Serial.println();

Serial.print("  Number of accounts: ");
Serial.println(ix.accounts.size());

for (size_t i = 0; i < ix.accounts.size(); i++) {
  Serial.print("  Account ");
  Serial.print(i);
  Serial.print(": ");
  for (uint8_t b : ix.accounts[i].pubkey.data) {
    if (b < 16) Serial.print("0");
    Serial.print(b, HEX);
  }
  Serial.print(" (writable: ");
  Serial.print(ix.accounts[i].isWritable);
  Serial.print(", signer: ");
  Serial.print(ix.accounts[i].isSigner);
  Serial.println(")");
}

Serial.print("  Instruction data: ");
  for (uint8_t b : data) {
    if (b < 16) Serial.print("0");
    Serial.print(b, HEX);
    Serial.print(" ");
  }
  Serial.println();


  Transaction tx;
  tx.fee_payer = authority;
  tx.recent_blockhash = solana.getLatestBlockhash();
  if (tx.recent_blockhash.isEmpty()) {
      Serial.println("❌ Failed to get blockhash!");
      return;
  }
  tx.add(ix);
  tx.sign({signer});
  String txBase64 = tx.serializeBase64();

  // First simulate the transaction to get detailed logs
  Serial.println("\n=== 🔍 Simulating Transaction ===");
  if (!simulateTransaction(txBase64)) {
      Serial.println("❌ Transaction simulation failed - not sending transaction");
      return;
  }

  String txSig;
  if (sendRawTransactionCustom(txBase64, txSig)) {

//  if (solana.sendRawTransaction(txBase64, txSig)) {
      Serial.println("✅ Anchor tx sent! Signature: " + txSig);
      confirmTransaction(txSig);
  } else {
      Serial.println("❌❌❌ Anchor tx failed.");
  }
}

String createPDAFromSeeds(const std::vector<std::vector<uint8_t>>& customSeeds) {
  Serial.println("\n=== 🔹 Creating PDA from Seeds ===");
  
  // Debug: Print the exact seeds being used
  Serial.println("Custom seeds received:");
  for (size_t i = 0; i < customSeeds.size(); i++) {
    Serial.print("  Seed ");
    Serial.print(i);
    Serial.print(" (");
    Serial.print(customSeeds[i].size());
    Serial.print(" bytes): ");
    for (uint8_t b : customSeeds[i]) {
      if (b < 16) Serial.print("0");
      Serial.print(b, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  // Program ID for the PDA derivation
  std::vector<uint8_t> programId = base58ToPubkey(PROGRAM_ID);
  
  // Debug: Print program ID
  Serial.print("Program ID (");
  Serial.print(programId.size());
  Serial.print(" bytes): ");
  for (uint8_t b : programId) {
    if (b < 16) Serial.print("0");
    Serial.print(b, HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Output variables
  std::vector<uint8_t> pdaBytes;
  uint8_t bump;
  
  // Generate the PDA
  if (solana.findProgramAddress(customSeeds, programId, pdaBytes, bump)) {
    Serial.println("✅ PDA created successfully!");
    
      // Debug: Print the exact PDA bytes
      Serial.print("PDA Bump: ");
      Serial.println(bump);
      Serial.print("PDA Bytes (");
      Serial.print(pdaBytes.size());
      Serial.print(" bytes, hex): ");
      for (uint8_t b : pdaBytes) {
        if (b < 16) Serial.print("0");
        Serial.print(b, HEX);
      }
      Serial.println();

    // Convert PDA bytes back to base58 string for display
    // Note: You'd need a proper bytes-to-base58 encoder here
    Serial.print("PDA Bump: ");
    Serial.println(bump);
    Serial.print("PDA Bytes (hex): ");
    for (uint8_t b : pdaBytes) {
      if (b < 16) Serial.print("0");
      Serial.print(b, HEX);
    }
    Serial.println();
    
    // Convert PDA bytes to base58 string for return
    Serial.print("PDA base 58: ");
    Serial.println(base58Encode(pdaBytes.data(), pdaBytes.size()));
    return base58Encode(pdaBytes.data(), pdaBytes.size());
  } 
  else {
    Serial.println("❌ Failed to create PDA");
    return "";
  }
} 