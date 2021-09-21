/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/brave_wallet_prefs.h"

#include <memory>
#include <utility>

#include "base/test/scoped_feature_list.h"
#include "brave/components/brave_wallet/browser/brave_wallet_constants.h"
#include "brave/components/brave_wallet/browser/pref_names.h"
#include "brave/components/brave_wallet/common/features.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {

class BraveWalletPrefsUnitTest : public testing::Test {
 public:
  BraveWalletPrefsUnitTest()
      : task_environment_(base::test::TaskEnvironment::TimeSource::MOCK_TIME) {}
  ~BraveWalletPrefsUnitTest() override = default;

 protected:
  void SetUp() override {
    TestingProfile::Builder builder;
    auto prefs =
        std::make_unique<sync_preferences::TestingPrefServiceSyncable>();
    // RegisterProfilePrefsForMigration(prefs->registry());
    RegisterUserProfilePrefs(prefs->registry());
    builder.SetPrefService(std::move(prefs));
    profile_ = builder.Build();
  }

  PrefService* GetPrefs() { return profile_->GetPrefs(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;
};

TEST_F(BraveWalletPrefsUnitTest,
       MigrateObsoleteBraveWalletProfilePrefsNativeWalletEnabled) {
  base::test::ScopedFeatureList feature_list;
  feature_list.InitAndEnableFeature(
      brave_wallet::features::kNativeBraveWalletFeature);

  // Wallet remains wallet
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::BRAVE_WALLET));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::BRAVE_WALLET,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));

  // MetaMask remains MetaMask
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::METAMASK));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::METAMASK,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));

  // Ask changes to BraveWallet
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::ASK));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::BRAVE_WALLET,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));

  // Crypto wallets changes to BraveWallet
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::CRYPTO_WALLETS));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::BRAVE_WALLET,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));
}

TEST_F(BraveWalletPrefsUnitTest,
       MigrateObsoleteBraveWalletProfilePrefsNativeWalletDisabled) {
  base::test::ScopedFeatureList feature_list;
  feature_list.InitAndDisableFeature(
      brave_wallet::features::kNativeBraveWalletFeature);

  // Wallet changes to ask
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::BRAVE_WALLET));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::ASK,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));

  // MetaMask remains MetaMask
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::METAMASK));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::METAMASK,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));

  // Ask stays at ask
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::ASK));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::ASK,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));

  // Crypto wallets remains Crypto wallets
  GetPrefs()->SetInteger(kBraveWalletWeb3ProviderDeprecated,
                         static_cast<int>(Web3ProviderTypes::CRYPTO_WALLETS));
  MigrateObsoleteBraveWalletProfilePrefs(GetPrefs());
  EXPECT_EQ(
      Web3ProviderTypes::CRYPTO_WALLETS,
      static_cast<Web3ProviderTypes>(GetPrefs()->GetInteger(kDefaultWallet)));
}

}  // namespace brave_wallet
