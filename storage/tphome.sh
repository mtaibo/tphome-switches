#!/bin/bash

# --- VARIABLES CONFIGURABLES ---
HOST="localhost"
DEVICE_ID="B364859"
LOCATION="secondary_bedroom"
DEVICE_TYPE="blind"

# Topics base
ADMIN_BASE="tphome/admin"
BLIND_BASE="tphome/blinds/$LOCATION/$DEVICE_TYPE"

# --- LÓGICA DEL SCRIPT ---
case "$1" in
--admin)
  if [ "$2" == "--sub" ]; then
    echo "Escuchando ADMIN: $ADMIN_BASE/+/state"
    mosquitto_sub -h "$HOST" -t "$ADMIN_BASE/+/state" -v
  elif [ "$2" == "--pub" ]; then
    MSG="$3"
    echo "Publicando en ADMIN: $MSG"
    mosquitto_pub -h "$HOST" -t "$ADMIN_BASE/$DEVICE_ID/set" -m "$MSG"
  fi
  ;;

--blind)
  if [ "$2" == "--sub" ]; then
    echo "Escuchando PERSIANA: $BLIND_BASE/state"
    mosquitto_sub -h "$HOST" -t "$BLIND_BASE/state" -v
  elif [ "$2" == "--pub" ]; then
    MSG="$3"
    echo "Publicando en PERSIANA: $MSG"
    mosquitto_pub -h "$HOST" -t "$BLIND_BASE/set" -m "$MSG"
  fi
  ;;

*)
  echo "Uso correcto:"
  echo "  tphome --admin [--sub | --pub 'MENSAJE']"
  echo "  tphome --blind [--sub | --pub 'MENSAJE']"
  ;;
esac
