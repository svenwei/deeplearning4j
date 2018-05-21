#! /bin/bash
BASEDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

function echoError() {
    (>&2 echo "$1")
}

function sparkError() {
    echoError "Changing Spark major version to 2 in the build did not change the state of your working copy, is Spark 1.x still the default ?"
    exit 2
}

function scalaError() {
    echoError "Changing Scala major version to 2.10 in the build did not change the state of your working copy, is Scala 2.11 still the default ?"
    exit 2
}

function whatchanged() {
    cd "$BASEDIR"
    for i in $(git status -s --porcelain -- $(find ./ -mindepth 2 -name pom.xml)|awk '{print $2}'); do
        echo "$(dirname $i)"
        cd "$BASEDIR"
    done
}

set -eu
./change-scala-versions.sh 2.11
./change-spark-versions.sh 1 # should be idempotent, this is the default
mvn "$@"
./change-spark-versions.sh 2
if [ -z "$(whatchanged)" ]; then
    sparkError;
else
    mvn -Dspark.major.version=2 -Dmaven.clean.skip=true -pl $(whatchanged| tr '\n' ',') -amd "$@"
fi
./change-scala-versions.sh 2.10
./change-spark-versions.sh 1
if [ -z "$(whatchanged)" ]; then
    scalaError;
else
    if [[ "${@#-pl}" = "$@" ]]; then
        mvn -Dmaven.clean.skip=true -pl $(whatchanged| tr '\n' ',') -amd "$@"
    else
        # the arguments already tweak the project list ! don't tweak them more
        # as this can lead to conflicts (excluding a project that's not part of
        # the reactor)
        mvn "$@"
    fi
fi
./change-scala-versions.sh 2.11 # back to the default
./change-spark-versions.sh 1
