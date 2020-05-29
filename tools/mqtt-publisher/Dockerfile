FROM node:14.3-alpine

WORKDIR /usr/app

COPY package*.json ./

RUN npm install

COPY mqtt-publisher.js .
COPY config.json .

ENTRYPOINT [ "node", "mqtt-publisher.js" ]
