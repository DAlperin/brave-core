/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/tokens/issuers/issuers_util.h"

#include "bat/ads/internal/tokens/issuers/issuers_info.h"
#include "bat/ads/internal/tokens/issuers/issuers_unittest_util.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsIssuersUtilTest, HasIssuersChanged) {
  // Arrange
  const IssuersInfo issuers_1 =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers_1);

  // Act
  const IssuersInfo issuers_2 =
      BuildIssuers(3600000,
                   {"Nj2NZ6nJUsK5MJ9ga9tfyctxzpT+GlvENF2TRHU4kBg=",
                    "TFQCiRJocOh0A8+qHQvdu3V/lDpGsZHJOnZzqny6rFg="},
                   {"PmXS59VTEVIPZckOqGdpjisDidUbhLGbhAhN5tmfhhs=",
                    "Bgk5gT+b96iSr3nD5nuTM/yGQ5klrIe6VC6DDdM6sFs=",
                    "zNWjpwIbghgXvTol3XPLKV3NJoEFtvUoPMiKstiWm3A="});

  const bool has_changed = HasIssuersChanged(issuers_2);

  // Assert
  EXPECT_TRUE(has_changed);
}

TEST(BatAdsIssuersUtilTest, HasIssuersNotChanged) {
  // Arrange
  const IssuersInfo issuers_1 =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers_1);

  // Act
  const IssuersInfo issuers_2 =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  const bool has_changed = HasIssuersChanged(issuers_2);

  // Assert
  EXPECT_FALSE(has_changed);
}

TEST(BatAdsIssuersUtilTest, IssuerDoesExistForConfirmationsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist = IssuerExistsForType(IssuerType::kConfirmations);

  // Assert
  EXPECT_TRUE(does_exist);
}

TEST(BatAdsIssuersUtilTest, IssuerDoesNotExistForConfirmationsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000, {},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist = IssuerExistsForType(IssuerType::kConfirmations);

  // Assert
  EXPECT_FALSE(does_exist);
}

TEST(BatAdsIssuersUtilTest, IssuerDoesExistForPaymentsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist = IssuerExistsForType(IssuerType::kPayments);

  // Assert
  EXPECT_TRUE(does_exist);
}

TEST(BatAdsIssuersUtilTest, IssuerDoesNotExistForPaymentsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                  {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                  {});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist = IssuerExistsForType(IssuerType::kPayments);

  // Assert
  EXPECT_FALSE(does_exist);
}

TEST(BatAdsIssuersUtilTest, PublickKeyDoesExistForConfirmationsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist =
      PublicKeyExistsForType(IssuerType::kConfirmations,
                             "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24=");

  // Assert
  EXPECT_TRUE(does_exist);
}

TEST(BatAdsIssuersUtilTest, PublickKeyDoesNotExistForConfirmationsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist =
      PublicKeyExistsForType(IssuerType::kConfirmations,
                             "Nj2NZ6nJUsK5MJ9ga9tfyctxzpT+GlvENF2TRHU4kBg=");

  // Assert
  EXPECT_FALSE(does_exist);
}

TEST(BatAdsIssuersUtilTest, PublickKeyDoesExistForPaymentsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist = PublicKeyExistsForType(
      IssuerType::kPayments, "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ=");

  // Assert
  EXPECT_TRUE(does_exist);
}

TEST(BatAdsIssuersUtilTest, PublickKeyDoesNotExistForPaymentsType) {
  // Arrange
  const IssuersInfo issuers =
      BuildIssuers(7200000,
                   {"JsvJluEN35bJBgJWTdW/8dAgPrrTM1I1pXga+o7cllo=",
                    "cKo0rk1iS8Obgyni0X3RRoydDIGHsivTkfX/TM1Xl24="},
                   {"JiwFR2EU/Adf1lgox+xqOVPuc6a/rxdy/LguFG5eaXg=",
                    "XgxwreIbLMu0IIFVk4TKEce6RduNVXngDmU3uixly0M=",
                    "CrQLMWmUuYog6Q93nScS8Lo1HHSex8WM2Qxij7qhjkQ="});

  ConfirmationsState::Get()->SetIssuers(issuers);

  // Act
  const bool does_exist = PublicKeyExistsForType(
      IssuerType::kPayments, "zNWjpwIbghgXvTol3XPLKV3NJoEFtvUoPMiKstiWm3A=");

  // Assert
  EXPECT_FALSE(does_exist);
}

}  // namespace ads
