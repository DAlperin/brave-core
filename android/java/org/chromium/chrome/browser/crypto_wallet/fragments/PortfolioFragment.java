/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.fragments;

import static org.chromium.chrome.browser.crypto_wallet.util.Utils.fromHexWei;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.base.Log;
import org.chromium.base.task.PostTask;
import org.chromium.brave_wallet.mojom.AccountInfo;
import org.chromium.brave_wallet.mojom.AssetPriceTimeframe;
import org.chromium.brave_wallet.mojom.AssetRatioController;
import org.chromium.brave_wallet.mojom.EthJsonRpcController;
import org.chromium.brave_wallet.mojom.KeyringController;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.crypto_wallet.activities.BraveWalletActivity;
import org.chromium.chrome.browser.crypto_wallet.adapters.WalletCoinAdapter;
import org.chromium.chrome.browser.crypto_wallet.listeners.OnWalletListItemClick;
import org.chromium.chrome.browser.crypto_wallet.model.WalletListItemModel;
import org.chromium.chrome.browser.crypto_wallet.util.SmoothLineChartEquallySpaced;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;
import org.chromium.content_public.browser.UiThreadTaskTraits;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;

public class PortfolioFragment
        extends Fragment implements OnWalletListItemClick, AdapterView.OnItemSelectedListener {
    private static String TAG = "PortfolioFragment";
    private Spinner mSpinner;
    private TextView mBalance;

    public static PortfolioFragment newInstance() {
        return new PortfolioFragment();
    }

    private EthJsonRpcController getEthJsonRpcController() {
        Activity activity = getActivity();
        if (activity instanceof BraveWalletActivity) {
            return ((BraveWalletActivity) activity).getEthJsonRpcController();
        }

        return null;
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
            @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_portfolio, container, false);

        view.setOnTouchListener(new View.OnTouchListener() {
            @Override
            @SuppressLint("ClickableViewAccessibility")
            public boolean onTouch(View v, MotionEvent event) {
                SmoothLineChartEquallySpaced chartES = view.findViewById(R.id.line_chart);
                if (chartES == null) {
                    return true;
                }
                if (event.getAction() == MotionEvent.ACTION_MOVE
                        || event.getAction() == MotionEvent.ACTION_DOWN) {
                    chartES.drawLine(event.getRawX(), null);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    chartES.drawLine(-1, null);
                }

                return true;
            }
        });

        mSpinner = view.findViewById(R.id.spinner);
        // class PortfolioFragment in fact is used both for Portfolio and Apps
        // screens, see CryptoFragmentPageAdapter.getItem.
        // Without post task with delay it happens that when 2nd instance is
        // created, then 1st instance onItemSelected is triggered with
        // position=0. That makes onItemSelected to switch network, but the
        // actual displayed value of spinner's item on 1st instance remains
        // unchanged.
        PostTask.postDelayedTask(UiThreadTaskTraits.DEFAULT,
                () -> { mSpinner.setOnItemSelectedListener(this); }, 500);

        mBalance = view.findViewById(R.id.balance);
        mBalance.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //UpdatePortfolio();
                UpdatePortfolioERC20();
            }
        });

        // Creating adapter for spinner
        ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.simple_spinner_item, Utils.getNetworksList(getActivity()));
        dataAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mSpinner.setAdapter(dataAdapter);

        return view;
    }

    private void updateNetwork() {
        EthJsonRpcController ethJsonRpcController = getEthJsonRpcController();
        if (ethJsonRpcController != null) {
            ethJsonRpcController.getChainId(chain_id -> {
                mSpinner.setSelection(getIndexOf(mSpinner, chain_id));
                //UpdatePortfolio();
            });
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        updateNetwork();
    }

    private int getIndexOf(Spinner spinner, String chain_id) {
        String strNetwork = Utils.getNetworkText(getActivity(), chain_id).toString();
        for (int i = 0; i < spinner.getCount(); i++) {
            if (spinner.getItemAtPosition(i).toString().equalsIgnoreCase(strNetwork)) {
                return i;
            }
        }

        return 0;
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        String item = parent.getItemAtPosition(position).toString();
        EthJsonRpcController ethJsonRpcController = getEthJsonRpcController();
        if (ethJsonRpcController != null) {
            ethJsonRpcController.setNetwork(Utils.getNetworkConst(getActivity(), item));
            //UpdatePortfolio();
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> arg0) {}

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        assert getActivity() != null;

        SmoothLineChartEquallySpaced chartES = view.findViewById(R.id.line_chart);
        chartES.setColors(new int[] {0xFFF73A1C, 0xFFBF14A2, 0xFF6F4CD2});
        chartES.setData(new float[] {15, 21, 9, 21, 25, 35, 24, 28});

        setUpCoinList(view);
        Button editVisibleAssets = view.findViewById(R.id.edit_visible_assets);
        editVisibleAssets.setOnClickListener(v -> {
            EditVisibleAssetsBottomSheetDialogFragment bottomSheetDialogFragment =
                    EditVisibleAssetsBottomSheetDialogFragment.newInstance(
                            WalletCoinAdapter.AdapterType.EDIT_VISIBLE_ASSETS_LIST);
            bottomSheetDialogFragment.show(
                    getFragmentManager(), EditVisibleAssetsBottomSheetDialogFragment.TAG_FRAGMENT);
        });
    }

    private void setUpCoinList(View view) {
        RecyclerView rvCoins = view.findViewById(R.id.rvCoins);
        WalletCoinAdapter walletCoinAdapter =
                new WalletCoinAdapter(WalletCoinAdapter.AdapterType.VISIBLE_ASSETS_LIST);
        List<WalletListItemModel> walletListItemModelList = new ArrayList<>();
        walletListItemModelList.add(new WalletListItemModel(
                R.drawable.ic_eth, "Ethereum", "ETH", "$872.48", "0.31178 ETH"));
        walletCoinAdapter.setWalletListItemModelList(walletListItemModelList);
        walletCoinAdapter.setOnWalletListItemClick(PortfolioFragment.this);
        walletCoinAdapter.setWalletListItemType(Utils.ASSET_ITEM);
        rvCoins.setAdapter(walletCoinAdapter);
        rvCoins.setLayoutManager(new LinearLayoutManager(getActivity()));
    }

    @Override
    public void onAssetClick() {
        Utils.openAssetDetailsActivity(getActivity());
    }

    private AssetRatioController getAssetRatioController() {
        Activity activity = getActivity();
        if (activity instanceof BraveWalletActivity) {
            return ((BraveWalletActivity) activity).getAssetRatioController();
        }

        return null;
    }

    private KeyringController getKeyringController() {
        Activity activity = getActivity();
        if (activity instanceof BraveWalletActivity) {
            return ((BraveWalletActivity) activity).getKeyringController();
        }

        return null;
    }

    // Helper for async EthJsonRpcController.getBalance
    class GetBalanceResponseContext implements EthJsonRpcController.GetBalanceResponse {
        public CompletableFuture<Void> future;
        public Boolean success;
        public String balance;

        public GetBalanceResponseContext() {
            future = new CompletableFuture<>();
        }

        @Override
        public void call(Boolean success, String balance) {
            this.success = success;
            this.balance = balance;
            future.complete(null);
        }
    }

    private void UpdatePortfolio() {
        AssetRatioController assetRatioController = getAssetRatioController();
        assert assetRatioController != null : "assetRatioController is null";

        // Get Ethereum price in USD
        assetRatioController.getPrice(new String[] {"eth"}, new String[] {"usd"},
                AssetPriceTimeframe.LIVE, (success, assetPrices) -> {
                    if (success && assetPrices.length == 1 && assetPrices[0].fromAsset.equals("eth")
                            && assetPrices[0].toAsset.equals("usd")) {
                        Double usdPerEth;
                        try {
                            usdPerEth = Double.parseDouble(assetPrices[0].price);
                        } catch (NullPointerException | NumberFormatException ex) {
                            Log.e(TAG, "Cannot parse " + assetPrices[0].price + ", " + ex);
                            return;
                        }

                        // Get all accounts
                        KeyringController keyringController = getKeyringController();
                        assert keyringController != null : "keyringController is null";
                        keyringController.getDefaultKeyringInfo(keyringInfo -> {
                            if (keyringInfo != null) {
                                AccountInfo[] accountInfos = keyringInfo.accountInfos;
                                EthJsonRpcController rpcController = getEthJsonRpcController();
                                assert rpcController != null : "rpcController is null";
                                ArrayList<GetBalanceResponseContext> contexts =
                                        new ArrayList<GetBalanceResponseContext>();
                                for (AccountInfo accountInfo : accountInfos) {
                                    GetBalanceResponseContext context =
                                            new GetBalanceResponseContext();
                                    contexts.add(context);
                                    rpcController.getBalance(accountInfo.address, context);
                                }

                                CompletableFuture<Void>[] futureArr = (CompletableFuture<
                                        Void>[]) new CompletableFuture[accountInfos.length];
                                for (int i = 0; i < contexts.size(); ++i) {
                                    futureArr[i] = contexts.get(i).future;
                                }
                                CompletableFuture<Void> all = CompletableFuture.allOf(futureArr);

                                all.thenRunAsync(() -> {
                                    Double fiatSum = 0.0d;
                                    for (GetBalanceResponseContext context : contexts) {
                                        fiatSum += ((context.success)
                                                        ? (fromHexWei(context.balance) * usdPerEth)
                                                        : 0.0d);
                                    }
                                    final String fiatSumString = String.format("$%,.2f", fiatSum);
                                    PostTask.runOrPostTask(UiThreadTaskTraits.DEFAULT, () -> {
                                        mBalance.setText(fiatSumString);
                                        mBalance.invalidate();
                                    });
                                });
                            } else {
                                Log.e(TAG, "Wrong keyring info");
                            }
                        });
                    } else {
                        Log.e(TAG, "Wrong response from AssetRatioController");
                    }
                });
    }


    private void UpdatePortfolioERC20() {
        Log.e(TAG, "UpdatePortfolioERC20() 000");
        /*
        ercTokenRegistry.getAllTokens(tokens -> { setUpAssetsList(view, tokens); });
        updateBuySendAsset(String asset, ErcToken ercToken)
        mCurrentErcToken = ercToken;

                address : all from
                mKeyringController.getDefaultKeyringInfo
                for (AccountInfo info : keyring.accountInfos) {
                  address_es = info.address;
                }

                if (mCurrentErcToken == null || mCurrentErcToken.contractAddress.isEmpty()) {
                    mEthJsonRpcController.getBalance(address, (success, balance) -> {
                        if (!success) {
                            return;
                        }
                        populateBalance(balance);
                    });
                } else {
                    mEthJsonRpcController.getErc20TokenBalance(
                            mCurrentErcToken.contractAddress, address, (success, balance) -> {
                                if (!success) {
                                    return;
                                }
                                populateBalance(balance);
                            });
                }
        */
        ;
        KeyringController keyringController = getKeyringController();
        assert keyringController != null : "keyringController should not be null";
        ErcTokenRegistry ercTokenRegistry = getErcTokenRegistry();
        assert ercTokenRegistry != null : "ercTokenRegistry should not be null";
        EthJsonRpcController rpcController = getEthJsonRpcController();
        assert rpcController != null : "rpcController is null";


        keyringController.getDefaultKeyringInfo(keyringInfo -> {
            Log.e(TAG, "getDefaultKeyringInfo cb ========================");
            Log.e(TAG, "getDefaultKeyringInfo cb 000 keyring.accountInfos.length=" + keyringInfo.accountInfos.length);
            for (AccountInfo info : keyringInfo.accountInfos) {
                Log.e(TAG, "info.address=" + info.address);
            }
            Log.e(TAG, "getDefaultKeyringInfo cb ========================");

            ercTokenRegistry.getAllTokens(tokens -> {
                Log.e(TAG, "getAllTokens cb ========================");
                Log.e(TAG, "getAllTokens cb 000 tokens.length=" + tokens.length);
                for (ErcToken token : tokens) {
                    if (token == null) { Log.e(TAG, "getAllTokens cb 002 token is null"); }
                    Log.e(TAG, "getAllTokens cb 002 token.contractAddress="+token.contractAddress);
                    Log.e(TAG, "getAllTokens cb 002 token.name           ="+token.name);
                    Log.e(TAG, "getAllTokens cb 002 token.logo           ="+token.logo);
                    Log.e(TAG, "getAllTokens cb 002 token.isErc20        ="+token.isErc20);
                    Log.e(TAG, "getAllTokens cb 002 token.isErc721       ="+token.isErc721);
                    Log.e(TAG, "getAllTokens cb 002 token.symbol         ="+token.symbol);
                    Log.e(TAG, "getAllTokens cb 002 token.decimals       ="+token.decimals);
                    Log.e(TAG, "getAllTokens cb 002 token.visible        ="+token.visible);
                    Log.e(TAG, "getAllTokens cb --------------");
                }
                Log.e(TAG, "getAllTokens cb ========================");


                Log.e(TAG, "Iterating all again ========================");
                for (AccountInfo info : keyringInfo.accountInfos) {
                    //Log.e(TAG, "info.address=" + info.address);
                    String address = info.address;

                    for (ErcToken token : tokens) {
                        if (token.contractAddress.isEmpty()) {
                            rpcController.getBalance(address, (success, balance) -> {
                                Log.e(TAG, "getBalance cb address="+address);
                                Log.e(TAG, "getBalance cb success="+success);
                                Log.e(TAG, "getBalance cb balance="+balance);
                                // if (!success) {
                                //     return;
                                // }
                                // populateBalance(balance);
                            });

                        } else {
                          rpcController.getErc20TokenBalance(
                                  token.contractAddress, address, (success, balance) -> {
                                      Log.e(TAG, "getErc20TokenBalance cb address="+address);
                                      Log.e(TAG, "getErc20TokenBalance cb success="+success);
                                      Log.e(TAG, "getErc20TokenBalance cb balance="+balance);
                                      // if (!success) {
                                      //     return;
                                      // }
                                      //populateBalance(balance);
                                  });

                        }
                    }
                }

            });  // ercTokenRegistry.getAllTokens


        }); // keyringController.getDefaultKeyringInfo



    }  // UpdatePortfolioERC20()
}
