/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { LocaleContext } from '../../../lib/locale_context'
import { WithThemeVariables } from '../../with_theme_variables'
import { RewardsCard } from '../rewards_card'

import { localeStrings } from './locale_strings'

const localeContext = {
  getString (key: string) {
    return localeStrings[key] || 'MISSING'
  }
}

export default {
  title: 'Rewards'
}

function actionLogger (name: string, ...args: any[]) {
  return (...args: any[]) => console.log(name, ...args)
}

export function NewTabCard () {
  return (
    <LocaleContext.Provider value={localeContext}>
      <WithThemeVariables>
        <div style={{ width: '284px' }}>
          <RewardsCard
            rewardsEnabled={true}
            adsEnabled={false}
            rewardsBalance={91.5812}
            exchangeCurrency='USD'
            exchangeRate={0.82}
            adGrantAmount={1.25}
            nextPaymentDate={Date.now() + 1000 * 60 * 60 * 24 * 1.2}
            earningsThisMonth={0.142}
            earningsLastMonth={1.25}
            contributionsThisMonth={10}
            onEnableRewards={actionLogger('onEnableRewards')}
            onEnableAds={actionLogger('onEnableAds')}
            onDismissGrant={actionLogger('onDismissGrant')}
            onClaimGrant={actionLogger('onClaimGrant')}
          />
        </div>
      </WithThemeVariables>
    </LocaleContext.Provider>
  )
}
