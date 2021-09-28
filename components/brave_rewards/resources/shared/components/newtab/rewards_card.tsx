import * as React from 'react'

import { LocaleContext, formatMessage } from '../../lib/locale_context'
import { BatIcon } from '../icons/bat_icon'
import { SettingsIcon } from '../icons/settings_icon'
import { CloseIcon } from '../icons/close_icon'
import { MoneyBagIcon } from '../icons/money_bag_icon'
import { InfoIcon } from './icons/info_icon'
import { TokenAmount } from '../token_amount'
import { ExchangeAmount } from '../exchange_amount'
import { NewTabLink } from '../new_tab_link'
import { getDaysUntilRewardsPayment } from '../../lib/pending_rewards'

import * as urls from '../../lib/rewards_urls'

import * as style from './rewards_card.style'

const nextPaymentDateFormatter = new Intl.DateTimeFormat(undefined, {
  day: 'numeric',
  month: 'short'
})

const monthDayFormatter = new Intl.DateTimeFormat(undefined, {
  day: 'numeric',
  month: 'short'
})

function renderDateRange () {
  const now = new Date()
  const start = new Date(now.getFullYear(), now.getMonth(), 1)
  const end = new Date(start.getFullYear(), start.getMonth() + 1, -1)
  return (
    <>{monthDayFormatter.format(start)} - {monthDayFormatter.format(end)}</>
  )
}

interface Props {
  rewardsEnabled: boolean
  adsEnabled: boolean
  rewardsBalance: number
  exchangeRate: number
  exchangeCurrency: string
  nextPaymentDate: number
  earningsThisMonth: number
  earningsLastMonth: number
  contributionsThisMonth: number
  adGrantAmount: number
  onEnableRewards: () => void
  onEnableAds: () => void
  onDismissGrant: () => void
  onClaimGrant: () => void
}

export function RewardsCard (props: Props) {
  const { getString } = React.useContext(LocaleContext)

  function renderTitle () {
    return (
      <style.title>
        <BatIcon />{getString('braveRewards')}
      </style.title>
    )
  }

  function renderGrantOverlay () {
    return (
      <style.grant>
        <style.grantHeader>
          <div>
            <MoneyBagIcon />{getString('adEarningsReceived')}
          </div>
          <div>
            <button onClick={props.onDismissGrant}>
              <CloseIcon />
            </button>
          </div>
        </style.grantHeader>
        <style.grantText>
          {
            formatMessage(getString('adsGrantText'), [
              <TokenAmount
                key='amount'
                amount={props.adGrantAmount}
                minimumFractionDigits={1}
              />
            ])
          }
        </style.grantText>
        <style.primaryAction>
          <button onClick={props.onClaimGrant}>
            {getString('claimEarnings')}
          </button>
        </style.primaryAction>
      </style.grant>
    )
  }

  function renderBalance () {
    if (props.adGrantAmount > 0) {
      return renderGrantOverlay()
    }

    return (
      <style.balance>
        <style.balanceTitle>{getString('tokenBalance')}</style.balanceTitle>
        <style.balanceAmount>
          <TokenAmount amount={props.rewardsBalance} />
        </style.balanceAmount>
        <style.balanceExchange>
          <style.balanceExchangeAmount>
            ≈&nbsp;
            <ExchangeAmount
              amount={props.rewardsBalance}
              rate={props.exchangeRate}
              currency={props.exchangeCurrency}
            />
          </style.balanceExchangeAmount>
          <style.balanceExchangeNote>
            This may vary based on market volatility.
          </style.balanceExchangeNote>
        </style.balanceExchange>
        {renderPendingRewards()}
      </style.balance>
    )
  }

  function renderPendingRewards () {
    if (props.earningsLastMonth <= 0) {
      return null
    }

    const days = getDaysUntilRewardsPayment(props.nextPaymentDate)
    if (!days) {
      return null
    }

    return (
      <style.pendingRewards>
        {
          formatMessage(getString('pendingRewardsMessage'), [
            <span key='amount'>
              <strong>+</strong>
              <TokenAmount
                amount={props.earningsLastMonth}
                minimumFractionDigits={1}
              />
            </span>,
            days
          ])
        }
      </style.pendingRewards>
    )
  }

  function renderRewardsOptIn () {
    return (
      <style.root>
        {renderTitle()}
        <style.rewardsOptIn>
          <style.rewardsOptInHeader>
            {getString('rewardsOptInHeader')}
          </style.rewardsOptInHeader>
          {getString('rewardsOptInText')}
        </style.rewardsOptIn>
        <style.primaryAction>
          <button onClick={props.onEnableRewards}>
            {getString('startUsingRewards')}
          </button>
        </style.primaryAction>
        <style.termsOfService>
          {
            formatMessage(getString('rewardsTermsOfService'), {
              tags: {
                $1: (content) => (
                  <span key='terms'>
                    <br />
                    <NewTabLink href={urls.termsOfServiceURL}>
                      {content}
                    </NewTabLink>
                  </span>
                ),
                $3: (content) => (
                  <NewTabLink key='privacy' href={urls.privacyPolicyURL}>
                    {content}
                  </NewTabLink>
                )
              }
            })
          }
        </style.termsOfService>
      </style.root>
    )
  }

  if (!props.rewardsEnabled) {
    return renderRewardsOptIn()
  }

  return (
    <style.root>
      {renderTitle()}
      {renderBalance()}
      <style.progressHeader>
        <style.progressHeaderText>
          <span className='date-range'>{renderDateRange()}</span>&nbsp;
          {getString('progress')}
        </style.progressHeaderText>
        <style.progressHeaderBorder />
      </style.progressHeader>
      {
        !props.adsEnabled &&
          <>
            <style.adsOptIn>
              {getString('rewardsOptInText')}
            </style.adsOptIn>
            <style.primaryAction>
              <button onClick={props.onEnableAds}>
                {getString('startUsingRewards')}
              </button>
            </style.primaryAction>
          </>
      }
      <style.progress>
        {
          props.adsEnabled &&
            <style.earning>
              <style.progressItemLabel>
                {getString('earning')}
                <style.earningInfo>
                  <InfoIcon />
                  <div className='tooltip'>
                    <style.earningTooltip>
                      {
                        formatMessage(getString('earningInfoText'), [
                          nextPaymentDateFormatter.format(props.nextPaymentDate)
                        ])
                      }
                    </style.earningTooltip>
                  </div>
                </style.earningInfo>
              </style.progressItemLabel>
              <style.progressItemAmount>
                <TokenAmount amount={props.earningsThisMonth} />
              </style.progressItemAmount>
            </style.earning>
        }
        <style.giving>
          <style.progressItemLabel>
            {getString('giving')}
          </style.progressItemLabel>
          <style.progressItemAmount>
            <TokenAmount
              amount={props.contributionsThisMonth}
              minimumFractionDigits={1}
            />
          </style.progressItemAmount>
        </style.giving>
      </style.progress>
      <style.settings>
        <NewTabLink href={urls.settingsURL}>
          <SettingsIcon />{getString('rewardsSettings')}
        </NewTabLink>
      </style.settings>
    </style.root>
  )
}
