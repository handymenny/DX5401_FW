#! /bin/bash

set -x

echo 'gitopal' | sudo -S service postfix restart

curl -k --header "PRIVATE-TOKEN:QNJPSqZts7UotNsxoJRX" "https://btc-git.zyxel.com/api/v4/projects/246/merge_requests?source_branch=$CI_COMMIT_BRANCH" -o mr_id.log >& /dev/null

mr_id=`grep -o '\"iid"\:[0-9]*' mr_id.log | head -n 1 |  awk -F':' '{print $2}'`

make tools/private/zyxel/prohibit-function-check/compile

cd staging_dir/host/bin/prohibit-function-check*

./prohibit-function-check -d -c ${CI_COMMIT_BRANCH}

if [ $? -eq 0 ] ;then
    echo "insecure function check pass"
else
    echo "insecure function check fail";
    exit -1; 
#    echo -e "Hi $GITLAB_USER_NAME,\n\nBranch Name:$CI_COMMIT_BRANCH\nFail Reason:[CI/CD Force Pass]prohibited function check fail\nJOB_ID=$CI_JOB_URL\nMR_ID=https://btc-git.zyxel.com/opal20/opalcicd/-/merge_requests/$mr_id\n\n\nBR,\nGitopal" | mail -s "[CI/CD Force Pass]prohibited function check fail" $GITLAB_USER_EMAIL,jason.wu@zyxel.com.tw,daniel.chen@zyxel.com.tw,hchang@zyxel.com.tw,joanne.lai@zyxel.com.tw
fi
