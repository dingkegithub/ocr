FROM node
WORKDIR /ocrweb
#RUN mkdir /ocrweb
ADD ocr.html /ocrweb/
ADD jq-3.4.1.min.js /ocrweb/
ADD ocr.js /ocrweb/
ADD package.json /ocrweb/
RUN cd /ocrweb/; \
    npm install express
EXPOSE 9999
ENTRYPOINT ["node", "ocr.js"]
