/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_BRAVE_BROWSER_COMMAND_CONTROLLER_H_
#define BRAVE_BROWSER_UI_BRAVE_BROWSER_COMMAND_CONTROLLER_H_

#include "brave/components/brave_vpn/buildflags/buildflags.h"
#include "brave/components/ipfs/buildflags/buildflags.h"
#include "brave/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"

#if BUILDFLAG(ENABLE_IPFS)
#include "components/prefs/pref_change_registrar.h"
#endif

#if BUILDFLAG(ENABLE_BRAVE_VPN)
#include "brave/components/brave_vpn/brave_vpn.mojom.h"
#include "mojo/public/cpp/bindings/receiver.h"
#endif

class BraveAppMenuBrowserTest;
class BraveBrowserCommandControllerTest;

// This namespace is needed for a chromium_src override
namespace chrome {

class BraveBrowserCommandController : public chrome::BrowserCommandController
#if BUILDFLAG(ENABLE_BRAVE_VPN)
    ,
                                      public brave_vpn::mojom::ServiceObserver
#endif
{
 public:
  explicit BraveBrowserCommandController(Browser* browser);
  ~BraveBrowserCommandController() override;

#if BUILDFLAG(ENABLE_TOR)
  void UpdateCommandForTor();
#endif

 private:
  friend class ::BraveAppMenuBrowserTest;
  friend class ::BraveBrowserCommandControllerTest;

  // Overriden from CommandUpdater:
  bool SupportsCommand(int id) const override;
  bool IsCommandEnabled(int id) const override;
  bool ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition,
      base::TimeTicks time_stamp = base::TimeTicks::Now()) override;
  void AddCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(CommandObserver* observer) override;
  bool UpdateCommandEnabled(int id, bool state) override;

#if BUILDFLAG(ENABLE_BRAVE_VPN)
  // brave_vpn::mojom::ServiceObserver overrides:
  void OnPurchasedStateChanged(brave_vpn::mojom::PurchasedState state) override;
  void OnConnectionStateChanged(
      brave_vpn::mojom::ConnectionState state) override {}
  void OnConnectionCreated() override {}
  void OnConnectionRemoved() override {}
#endif

  void InitBraveCommandState();
  void UpdateCommandForBraveRewards();
  void UpdateCommandForBraveAdblock();
  void UpdateCommandForWebcompatReporter();
  void UpdateCommandForBraveSync();
  void UpdateCommandForBraveWallet();
  void UpdateCommandForSidebar();
  void UpdateCommandForBraveVPN();
  bool ExecuteBraveCommandWithDisposition(int id,
                                          WindowOpenDisposition disposition,
                                          base::TimeTicks time_stamp);

  Browser* const browser_;

  CommandUpdaterImpl brave_command_updater_;

#if BUILDFLAG(ENABLE_BRAVE_VPN)
  mojo::Receiver<brave_vpn::mojom::ServiceObserver> receiver_{this};
#endif

  DISALLOW_COPY_AND_ASSIGN(BraveBrowserCommandController);
};

}   // namespace chrome

#endif  // BRAVE_BROWSER_UI_BRAVE_BROWSER_COMMAND_CONTROLLER_H_
