/**
 * This code was AUTOGENERATED using the codama library.
 * Please DO NOT EDIT THIS FILE, instead use visitors
 * to add features, then rerun codama to update it.
 *
 * @see https://github.com/codama-idl/codama
 */

import {
  combineCodec,
  fixDecoderSize,
  fixEncoderSize,
  getAddressEncoder,
  getBytesDecoder,
  getBytesEncoder,
  getProgramDerivedAddress,
  getStructDecoder,
  getStructEncoder,
  getU64Decoder,
  getU64Encoder,
  transformEncoder,
  type Address,
  type Codec,
  type Decoder,
  type Encoder,
  type IAccountMeta,
  type IAccountSignerMeta,
  type IInstruction,
  type IInstructionWithAccounts,
  type IInstructionWithData,
  type ReadonlyAccount,
  type ReadonlyUint8Array,
  type TransactionSigner,
  type WritableAccount,
  type WritableSignerAccount,
} from 'gill';
import { VAULT_PROGRAM_ADDRESS } from '../programs';
import {
  expectAddress,
  getAccountMetaFactory,
  type ResolvedAccount,
} from '../shared';

export const DEPOSIT_DISCRIMINATOR = new Uint8Array([
  242, 35, 198, 137, 82, 225, 242, 182,
]);

export function getDepositDiscriminatorBytes() {
  return fixEncoderSize(getBytesEncoder(), 8).encode(DEPOSIT_DISCRIMINATOR);
}

export type DepositInstruction<
  TProgram extends string = typeof VAULT_PROGRAM_ADDRESS,
  TAccountUser extends string | IAccountMeta<string> = string,
  TAccountVaultState extends string | IAccountMeta<string> = string,
  TAccountVault extends string | IAccountMeta<string> = string,
  TAccountSystemProgram extends
    | string
    | IAccountMeta<string> = '11111111111111111111111111111111',
  TRemainingAccounts extends readonly IAccountMeta<string>[] = [],
> = IInstruction<TProgram> &
  IInstructionWithData<Uint8Array> &
  IInstructionWithAccounts<
    [
      TAccountUser extends string
        ? WritableSignerAccount<TAccountUser> & IAccountSignerMeta<TAccountUser>
        : TAccountUser,
      TAccountVaultState extends string
        ? ReadonlyAccount<TAccountVaultState>
        : TAccountVaultState,
      TAccountVault extends string
        ? WritableAccount<TAccountVault>
        : TAccountVault,
      TAccountSystemProgram extends string
        ? ReadonlyAccount<TAccountSystemProgram>
        : TAccountSystemProgram,
      ...TRemainingAccounts,
    ]
  >;

export type DepositInstructionData = {
  discriminator: ReadonlyUint8Array;
  amount: bigint;
};

export type DepositInstructionDataArgs = { amount: number | bigint };

export function getDepositInstructionDataEncoder(): Encoder<DepositInstructionDataArgs> {
  return transformEncoder(
    getStructEncoder([
      ['discriminator', fixEncoderSize(getBytesEncoder(), 8)],
      ['amount', getU64Encoder()],
    ]),
    (value) => ({ ...value, discriminator: DEPOSIT_DISCRIMINATOR })
  );
}

export function getDepositInstructionDataDecoder(): Decoder<DepositInstructionData> {
  return getStructDecoder([
    ['discriminator', fixDecoderSize(getBytesDecoder(), 8)],
    ['amount', getU64Decoder()],
  ]);
}

export function getDepositInstructionDataCodec(): Codec<
  DepositInstructionDataArgs,
  DepositInstructionData
> {
  return combineCodec(
    getDepositInstructionDataEncoder(),
    getDepositInstructionDataDecoder()
  );
}

export type DepositAsyncInput<
  TAccountUser extends string = string,
  TAccountVaultState extends string = string,
  TAccountVault extends string = string,
  TAccountSystemProgram extends string = string,
> = {
  user: TransactionSigner<TAccountUser>;
  vaultState?: Address<TAccountVaultState>;
  vault?: Address<TAccountVault>;
  systemProgram?: Address<TAccountSystemProgram>;
  amount: DepositInstructionDataArgs['amount'];
};

export async function getDepositInstructionAsync<
  TAccountUser extends string,
  TAccountVaultState extends string,
  TAccountVault extends string,
  TAccountSystemProgram extends string,
  TProgramAddress extends Address = typeof VAULT_PROGRAM_ADDRESS,
>(
  input: DepositAsyncInput<
    TAccountUser,
    TAccountVaultState,
    TAccountVault,
    TAccountSystemProgram
  >,
  config?: { programAddress?: TProgramAddress }
): Promise<
  DepositInstruction<
    TProgramAddress,
    TAccountUser,
    TAccountVaultState,
    TAccountVault,
    TAccountSystemProgram
  >
> {
  // Program address.
  const programAddress = config?.programAddress ?? VAULT_PROGRAM_ADDRESS;

  // Original accounts.
  const originalAccounts = {
    user: { value: input.user ?? null, isWritable: true },
    vaultState: { value: input.vaultState ?? null, isWritable: false },
    vault: { value: input.vault ?? null, isWritable: true },
    systemProgram: { value: input.systemProgram ?? null, isWritable: false },
  };
  const accounts = originalAccounts as Record<
    keyof typeof originalAccounts,
    ResolvedAccount
  >;

  // Original args.
  const args = { ...input };

  // Resolve default values.
  if (!accounts.vaultState.value) {
    accounts.vaultState.value = await getProgramDerivedAddress({
      programAddress,
      seeds: [
        getBytesEncoder().encode(new Uint8Array([115, 116, 97, 116, 101])),
        getAddressEncoder().encode(expectAddress(accounts.user.value)),
      ],
    });
  }
  if (!accounts.vault.value) {
    accounts.vault.value = await getProgramDerivedAddress({
      programAddress,
      seeds: [
        getBytesEncoder().encode(new Uint8Array([118, 97, 117, 108, 116])),
        getAddressEncoder().encode(expectAddress(accounts.vaultState.value)),
      ],
    });
  }
  if (!accounts.systemProgram.value) {
    accounts.systemProgram.value =
      '11111111111111111111111111111111' as Address<'11111111111111111111111111111111'>;
  }

  const getAccountMeta = getAccountMetaFactory(programAddress, 'programId');
  const instruction = {
    accounts: [
      getAccountMeta(accounts.user),
      getAccountMeta(accounts.vaultState),
      getAccountMeta(accounts.vault),
      getAccountMeta(accounts.systemProgram),
    ],
    programAddress,
    data: getDepositInstructionDataEncoder().encode(
      args as DepositInstructionDataArgs
    ),
  } as DepositInstruction<
    TProgramAddress,
    TAccountUser,
    TAccountVaultState,
    TAccountVault,
    TAccountSystemProgram
  >;

  return instruction;
}

export type DepositInput<
  TAccountUser extends string = string,
  TAccountVaultState extends string = string,
  TAccountVault extends string = string,
  TAccountSystemProgram extends string = string,
> = {
  user: TransactionSigner<TAccountUser>;
  vaultState: Address<TAccountVaultState>;
  vault: Address<TAccountVault>;
  systemProgram?: Address<TAccountSystemProgram>;
  amount: DepositInstructionDataArgs['amount'];
};

export function getDepositInstruction<
  TAccountUser extends string,
  TAccountVaultState extends string,
  TAccountVault extends string,
  TAccountSystemProgram extends string,
  TProgramAddress extends Address = typeof VAULT_PROGRAM_ADDRESS,
>(
  input: DepositInput<
    TAccountUser,
    TAccountVaultState,
    TAccountVault,
    TAccountSystemProgram
  >,
  config?: { programAddress?: TProgramAddress }
): DepositInstruction<
  TProgramAddress,
  TAccountUser,
  TAccountVaultState,
  TAccountVault,
  TAccountSystemProgram
> {
  // Program address.
  const programAddress = config?.programAddress ?? VAULT_PROGRAM_ADDRESS;

  // Original accounts.
  const originalAccounts = {
    user: { value: input.user ?? null, isWritable: true },
    vaultState: { value: input.vaultState ?? null, isWritable: false },
    vault: { value: input.vault ?? null, isWritable: true },
    systemProgram: { value: input.systemProgram ?? null, isWritable: false },
  };
  const accounts = originalAccounts as Record<
    keyof typeof originalAccounts,
    ResolvedAccount
  >;

  // Original args.
  const args = { ...input };

  // Resolve default values.
  if (!accounts.systemProgram.value) {
    accounts.systemProgram.value =
      '11111111111111111111111111111111' as Address<'11111111111111111111111111111111'>;
  }

  const getAccountMeta = getAccountMetaFactory(programAddress, 'programId');
  const instruction = {
    accounts: [
      getAccountMeta(accounts.user),
      getAccountMeta(accounts.vaultState),
      getAccountMeta(accounts.vault),
      getAccountMeta(accounts.systemProgram),
    ],
    programAddress,
    data: getDepositInstructionDataEncoder().encode(
      args as DepositInstructionDataArgs
    ),
  } as DepositInstruction<
    TProgramAddress,
    TAccountUser,
    TAccountVaultState,
    TAccountVault,
    TAccountSystemProgram
  >;

  return instruction;
}

export type ParsedDepositInstruction<
  TProgram extends string = typeof VAULT_PROGRAM_ADDRESS,
  TAccountMetas extends readonly IAccountMeta[] = readonly IAccountMeta[],
> = {
  programAddress: Address<TProgram>;
  accounts: {
    user: TAccountMetas[0];
    vaultState: TAccountMetas[1];
    vault: TAccountMetas[2];
    systemProgram: TAccountMetas[3];
  };
  data: DepositInstructionData;
};

export function parseDepositInstruction<
  TProgram extends string,
  TAccountMetas extends readonly IAccountMeta[],
>(
  instruction: IInstruction<TProgram> &
    IInstructionWithAccounts<TAccountMetas> &
    IInstructionWithData<Uint8Array>
): ParsedDepositInstruction<TProgram, TAccountMetas> {
  if (instruction.accounts.length < 4) {
    // TODO: Coded error.
    throw new Error('Not enough accounts');
  }
  let accountIndex = 0;
  const getNextAccount = () => {
    const accountMeta = instruction.accounts![accountIndex]!;
    accountIndex += 1;
    return accountMeta;
  };
  return {
    programAddress: instruction.programAddress,
    accounts: {
      user: getNextAccount(),
      vaultState: getNextAccount(),
      vault: getNextAccount(),
      systemProgram: getNextAccount(),
    },
    data: getDepositInstructionDataDecoder().decode(instruction.data),
  };
}
