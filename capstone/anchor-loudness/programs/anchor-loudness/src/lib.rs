#![allow(unexpected_cfgs, deprecated)]


use anchor_lang::prelude::*;
use ephemeral_rollups_sdk::anchor::{commit, ephemeral};
use ephemeral_rollups_sdk::cpi::DelegateConfig;
use ephemeral_rollups_sdk::ephem::{commit_accounts, commit_and_undelegate_accounts};

declare_id!("95jTSWUMS6XQWZxPBwNxhyTJmTZBf7sFCWoPBqsgRK2n");

mod state;
mod instructions;
mod errors;

pub use instructions::*;
pub use state::*;
pub use errors::*;

pub const TEST_PDA_SEED: &[u8] = b"test-pda";


#[ephemeral]
#[program]
pub mod anchor_loudness {
    use super::*;

    pub fn initialize_config(ctx: Context<InitializeConfig>) -> Result<()> {
        ctx.accounts.initialize_config(&ctx.bumps)
    }

    pub fn initialize_user(ctx: Context<InitializeUser>) -> Result<()> {
        ctx.accounts.initialize_user(&ctx.bumps)
    }

    pub fn get_sound_level<'a>(ctx: Context<SwitchboardFeed>) -> Result<()> {
        ctx.accounts.get_feed_data()?;
        Ok(())
    }

    pub fn create_submission(ctx: Context<CreateSubmission>, 
        _venue_name: String,
        sound_level_data: SoundLevelData,
    ) -> Result<()> {
        ctx.accounts.create_submission(sound_level_data, &ctx.bumps)
    }

    /// Delegate the account to the delegation program
    pub fn delegate(ctx: Context<DelegateInput>) -> Result<()> {
        ctx.accounts.delegate_pda(
            &ctx.accounts.payer,
            &[TEST_PDA_SEED],
            DelegateConfig::default(),
        )?;
        Ok(())
    }

    pub fn claim(ctx: Context<Claim>) -> Result<()> {
        ctx.accounts.claim()
    }

    pub fn close_submission(ctx: Context<CloseSubmission>, _venue_name: String) -> Result<()> {
        ctx.accounts.close_submission()
    } 

    pub fn close_user(ctx: Context<CloseUser>) -> Result<()> {
        ctx.accounts.close_user(&ctx.bumps)
    }

    pub fn close_config(ctx: Context<CloseConfig>) -> Result<()> {
        ctx.accounts.close_config(&ctx.bumps)
    }
}


