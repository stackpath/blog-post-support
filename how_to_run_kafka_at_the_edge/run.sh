#!/bin/bash

# shellcheck disable=SC1091

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace # Uncomment this line for debugging purpose

# Load libraries
. /opt/bitnami/scripts/libkafka.sh
. /opt/bitnami/scripts/libos.sh

##### Constants
BOOTSTRAP_SERVER=localhost:9092
REPLICATION_FACTOR=1
PARTITIONS=1
STARTED_SIGNAL="started (kafka.server.KafkaServer)"

# Load Kafka environment variables
eval "$(kafka_env)"

if [[ "${KAFKA_CFG_LISTENERS:-}" =~ SASL ]] || [[ "${KAFKA_CFG_LISTENER_SECURITY_PROTOCOL_MAP:-}" =~ SASL ]]; then
    export KAFKA_OPTS="-Djava.security.auth.login.config=${KAFKA_CONF_DIR}/kafka_jaas.conf"
fi

flags=("$KAFKA_CONF_FILE")
[[ -z "${KAFKA_EXTRA_FLAGS:-}" ]] || flags=("${flags[@]}" "${KAFKA_EXTRA_FLAGS[@]}")
START_COMMAND=("$KAFKA_HOME/bin/kafka-server-start.sh" "${flags[@]}")

info "** Starting Kafka **"
if am_i_root; then
    grep -q "$STARTED_SIGNAL" <(exec gosu "$KAFKA_DAEMON_USER" "${START_COMMAND[@]}")
else
    grep -q "$STARTED_SIGNAL" <(exec "${START_COMMAND[@]}")
fi

  info "** CHECKING TOPIC **"
if [ "$CREATE_TOPIC" = "yes" ]
then
  ./opt/bitnami/kafka/bin/kafka-topics.sh --create \
  --bootstrap-server $BOOTSTRAP_SERVER \
  --replication-factor $REPLICATION_FACTOR \
  --partitions $PARTITIONS \
  --topic $TOPIC || true
fi
  info "** CHECKING ROLE **"
if [ $KAFKA_ROLE = "CONSUME" ]
then
  info "** Starting Kafka Console Consumer **"
  ./opt/bitnami/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic $TOPIC --from-beginning
else
  info "** Starting Kafka Console Producer **"
  echo "THIS IS A TEST MESSAGE FOR THE TOPIC - $TOPIC" |  ./opt/bitnami/kafka/bin/kafka-console-producer.sh --broker-list localhost:9092 --topic $TOPIC
fi
