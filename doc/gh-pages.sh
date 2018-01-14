#!/bin/sh

[ -z $GH_REPO_REF ] && { echo "GH_REPO_REF is not set" 1>&2; exit 1; }

set -e

rm -rf gh-pages
# git clone -b gh-pages https://$GH_REPO_REF gh-pages
git clone -b hw0 https://$GH_REPO_REF gh-pages
cd gh-pages

# git config --global push.default simple
# git config user.name "ci agent"
# git config user.email "ci@localhost"

# rm -rf *

echo "" > .nojekyll

[ -d ../../html ] && mv ../../html .

if [ -d "html" ] && [ -f "html/index.html" ]; then

    echo 'deploy doc'
#    git add --all
#    git commit -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"
#    git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1
else
    echo 'doc build failed' >&2
    exit 1
fi

exit 0
