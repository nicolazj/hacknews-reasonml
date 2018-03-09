let s2e = ReasonReact.stringToElement;

let dangerousHtml: string => Js.t('a) = html => {"__html": html};