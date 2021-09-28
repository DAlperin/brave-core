import styled from 'styled-components'

import * as mixins from '../../lib/css_mixins'

export const root = styled.div`
  background: rgba(59, 62, 79, 0.9);
  backdrop-filter: blur(27.1828px);
  border-radius: 12px;
  color: var(--brave-palette-white);
  font-family: var(--brave-font-heading);
  padding: 14px 18px;
`

export const title = styled.div`
  font-weight: 600;
  font-size: 18px;
  line-height: 22px;

  .icon {
    height: 22px;
    width: auto;
    margin-right: 12px;
    margin-bottom: 2px;
    vertical-align: middle;
  }
`

export const rewardsOptIn = styled.div`
  margin-top: 11px;
  margin-bottom: 16px;
  color: var(--brave-palette-grey000);
  font-weight: 500;
  font-size: 12px;
  line-height: 18px;
  text-align: center;
  letter-spacing: 0.01em;
`

export const rewardsOptInHeader = styled.div`
  margin-bottom: 12px;
  font-weight: 600;
  font-size: 13px;
  line-height: 20px;
`

export const termsOfService = styled.div`
  margin-top: 14px;
  padding-bottom: 10px;
  font-weight: 500;
  font-size: 12px;
  line-height: 18px;
  text-align: center;
  letter-spacing: 0.01em;
  color: #D3D6E5;

  a {
    color: var(--brave-palette-blurple400);
  }
`

export const grant = styled.div`
  margin-top: 16px;
  background: #17171F;
  border-radius: 8px;
  padding: 10px 12px 16px;
`

export const grantHeader = styled.div`
  display: flex;
  font-weight: 600;
  font-size: 13px;
  line-height: 20px;
  letter-spacing: 0.01em;
  color: var(--brave-palette-grey000);

  > :first-child {
    flex: 1 1 auto;

    .icon {
      color: var(--brave-palette-yellow500);
      height: 16px;
      width: auto;
      vertical-align: middle;
      margin-right: 9px;
      margin-bottom: 2px;
    }
  }

  button {
    ${mixins.buttonReset}
    color: var(--brave-palette-grey400);
    cursor: pointer;

    .icon {
      height: 11px;
      width: auto;
    }

    &:hover {
      color: var(--brave-palette-grey200);
    }
  }
`

export const grantText = styled.div`
  margin-top: 8px;
  margin-bottom: 12px;
  font-size: 13px;
  line-height: 16px;
  color: var(--brave-palette-grey400);

  .amount, .currency {
    font-weight: 600;
  }
`

export const balance = styled.div`
  margin-top: 16px;
  font-size: 12px;
  line-height: 18px;
`

export const balanceTitle = styled.div`
  color: rgba(255, 255, 255, 0.8);
`

export const balanceAmount = styled.div`
  .amount {
    font-size: 36px;
    line-height: 40px;
  }

  .currency {
    font-size: 14px;
    line-height: 21px;
  }
`

export const balanceExchange = styled.div`
  color: rgba(255, 255, 255, 0.8);
  display: flex;
  gap: 6px;
  min-height: 24px;
`

export const balanceExchangeAmount = styled.div`
  flex: 0 0 auto;
`

export const balanceExchangeNote = styled.div`
  flex: 1 1 auto;
`

export const pendingRewards = styled.div`
  margin-top: 8px;
  background: var(--brave-palette-white);
  box-shadow: 0px 0px 24px rgba(99, 105, 110, 0.36);
  padding: 6px 12px;
  border-radius: 6px;
  color: var(--brave-palette-neutral900);
  font-size: 13px;
  line-height: 18px;
  text-align: center;

  strong, .amount {
    font-weight: 600;
  }
`

export const progressHeader = styled.div`
  margin-top: 16px;
  display: flex;
  align-items: center;
  gap: 11px;
  color: rgba(255, 255, 255, 0.8);
`

export const progressHeaderText = styled.div`
  .date-range {
    font-weight: 600;
    color: var(--brave-palette-white);
  }
`

export const progressHeaderBorder = styled.div`
  flex: 1 1 auto;
  border-top: 1px solid rgba(255, 255, 255, 0.3);
  height: 0px;
`

export const dateRange = styled.span`
  font-weight: 600;
  color: var(--brave-palette-white);
`

export const adsOptIn = styled.div`
  margin: 8px 0 15px;
  color: var(--brave-palette-grey000);
  font-weight: 500;
  font-size: 12px;
  line-height: 18px;
  letter-spacing: 0.01em;
`

export const primaryAction = styled.div`
  text-align: center;

  button {
    ${mixins.buttonReset}
    font-weight: 600;
    font-size: 12px;
    line-height: 24px;
    cursor: pointer;
    background: var(--brave-palette-blurple400);
    border-radius: 15px;
    padding: 4px 18px;

    &:hover {
      background: var(--brave-palette-blurple500);
    }
  }
`

export const progress = styled.div`
  margin-top: 12px;
  display: flex;
`

export const earning = styled.div`
  flex: 1 1 50%;
`

export const earningInfo = styled.span`
  position: relative;

  .tooltip {
    position: absolute;
    bottom: 100%;
    left: -38px;
    width: 168px;
    padding-bottom: 12px;
    display: none;
  }

  &:hover {
    .tooltip {
      display: initial;
    }
  }
`

export const earningTooltip = styled.div`
  position: relative;
  background: var(--brave-palette-blue500);
  box-shadow: 0px 0px 24px rgba(99, 105, 110, 0.36);
  padding: 12px;
  border-radius: 6px;
  color: var(--brave-palette-white);
  font-size: 12px;
  line-height: 20px;
  letter-spacing: 0.01em;

  &:before {
    content: '';
    position: absolute;
    bottom: -8px;
    left: 41px;
    background: inherit;
    height: 15px;
    width: 15px;
    transform: rotate(45deg);
  }
`

export const giving = styled.div`
  flex: 1 1 50%;
`

export const progressItemLabel = styled.div`
  color: rgba(255, 255, 255, 0.8);

  .icon {
    height: 14px;
    width: auto;
    vertical-align: middle;
    margin-left: 4px;
    margin-bottom: 2px;
  }
`

export const progressItemAmount = styled.div`
  font-size: 24px;
  line-height: 36px;

  .currency {
    font-size: 14px;
    line-height: 21px;
  }
`

export const settings = styled.div`
  margin-top: 13px;

  a {
    color: var(--brave-palette-neutral200);

    &:hover {
      color: var(--brave-palette-white);
    }
  }

  .icon {
    width: 15px;
    height: auto;
    vertical-align: middle;
    margin-right: 6px;
    margin-bottom: 2.5px;
  }
`
