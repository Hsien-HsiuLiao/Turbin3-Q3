import { Commitment, Connection, Keypair, LAMPORTS_PER_SOL, PublicKey } from "@solana/web3.js"
import wallet from "../../../Turbin3-wallet.json"
import { getOrCreateAssociatedTokenAccount, transfer } from "@solana/spl-token";

// We're going to import our keypair from the wallet file
const keypair = Keypair.fromSecretKey(new Uint8Array(wallet));

//Create a Solana devnet connection
const commitment: Commitment = "confirmed";
const connection = new Connection("https://api.devnet.solana.com", commitment);

// Mint address
const mint = new PublicKey("6jocG9wDorGF2Y18T7UrhFvJmYN9e36tfkd83VsuXW4V");

// Recipient address
const to = new PublicKey("3BLGfBreHxo1hUY2fdAiYGBqms3eMqiMCLdJGvXxhQ6V");

(async () => {
    try {
        // Get the token account of the fromWallet address, and if it does not exist, create it
        const fromATA = await getOrCreateAssociatedTokenAccount(
                                connection,
                                keypair,
                                mint,
                                keypair.publicKey,
        )

        // Get the token account of the toWallet address, and if it does not exist, create it
        const toATA = await getOrCreateAssociatedTokenAccount(
            connection,
            keypair,
            mint,
            to,
        )
        // Transfer the new token to the "toTokenAccount" we just created
        const tx = await transfer(
            connection,
            keypair,
            fromATA.address,
            toATA.address,
            keypair,
            .002*LAMPORTS_PER_SOL
        )
        console.log(`Transaction: ${tx}`);
    } catch(e) {
        console.error(`Oops, something went wrong: ${e}`)
    }
})();