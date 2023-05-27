# `mcharts`
a command-line tool to view real-time music chart data

## notes
- billboard link: https://www.billboard.com/charts/hot-100/

- xpath:
  - artist info: 
  - `//*[@id="post-1479786"]/div[3]/div/div/div/div[2]/div[3]/ul/li[4]/ul/li[1]/span` == Lil Durk Featuring J. Cole
  - `//*[@id="post-1479786"]/div[3]/div/div/div/div[2]/div[3]/ul/li[4]/ul/li[1]/h3` == all my life

  - //*[@id="post-1479786"]/div[3]/div/div/div/div[2]/div[4]/ul/li[4]/ul/li[1]/h3 == FLOWERS
  // miley:
  - //*[@id="post-1479786"]/div[3]/div/div/div/div[2]/div[4]/ul/li[4]/ul/li[1]/span

  - //*[@id="post-1479786"]/div[3]/div/div/div/div[2]/div[110]/ul/li[4]/ul/li[1]/span
  - //*[@id="post-1479786"]/div[3]/div/div/div/div[2]/div[13]/ul/li[4]/ul/li[1]/span == die for you



PATH: //li[@class=\"o-chart-results-list__item\"]/h3[@id=\"title-of-a-story\"]

<li class="o-chart-results-list__item // lrv-u-flex-grow-1 lrv-u-flex lrv-u-flex-direction-column lrv-u-justify-content-center lrv-u-border-b-1 u-border-b-0@mobile-max lrv-u-border-color-grey-light lrv-u-padding-l-050 lrv-u-padding-l-1@mobile-max">
											<h3 id="title-of-a-story"

== kill bill




////

//span[@class=\"c-label  a-no-trucate a-font-primary-s lrv-u-font-size-14@mobile-max u-line-height-normal@mobile-max u-letter-spacing-0021 lrv-u-display-block a-truncate-ellipsis-2line u-max-width-330 u-max-width-230@tablet-only\"]

<span class="c-label  a-no-trucate a-font-primary-s lrv-u-font-size-14@mobile-max u-line-height-normal@mobile-max u-letter-spacing-0021 lrv-u-display-block a-truncate-ellipsis-2line u-max-width-330 u-max-width-230@tablet-only" >



/////
old selectors:
// #define XPATH_TRACK "//li[@class=\"o-chart-results-list__item // lrv-u-flex-grow-1 lrv-u-flex lrv-u-flex-direction-column lrv-u-justify-content-center lrv-u-border-b-1 u-border-b-0@mobile-max lrv-u-border-color-grey-light lrv-u-padding-l-050 lrv-u-padding-l-1@mobile-max\"]/h3[@id=\"title-of-a-story\"]|" \
//                     "//li[@class=\"o-chart-results-list__item // lrv-u-flex-grow-1 lrv-u-flex lrv-u-flex-direction-column lrv-u-justify-content-center lrv-u-border-b-1 u-border-b-0@mobile-max lrv-u-border-color-grey-light lrv-u-padding-l-1@mobile-max\"]/h3[@id=\"\title-of-a-story\"]"
// #define XPATH_ARTIST "//span[@class=\"c-label  a-no-trucate a-font-primary-s lrv-u-font-size-14@mobile-max u-line-height-normal@mobile-max u-letter-spacing-0021 lrv-u-display-block a-truncate-ellipsis-2line u-max-width-330 u-max-width-230@tablet-only\"]"
//