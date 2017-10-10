FROM ubuntu:16.04
LABEL maintainer="Aaron Bloomfield aaron@virginia.edu"
ENV DEBIAN_FRONTEND noninteractive

# step 1: allow the use of a apt-cacher-ng proxy defined via
# --build-arg (this is only for the build, and is removed at the end)
ARG APT_PROXY=0.0.0.0
RUN echo $APT_PROXY | grep -v 0.0.0.0 | awk '{print "Acquire::http::Proxy \"http://" $1 ":3142\";"}' > /etc/apt/apt.conf.d/02proxy && \
    apt-get update -y -q -q

# step 2: a few things that are needed for this script to work well in docker
RUN apt-get dist-upgrade -y -q -q && \
    apt-get install -y -q -q --no-install-recommends supervisor net-tools locales apt-utils software-properties-common && \
    locale-gen en_US.UTF-8 && \
    apt-get clean -y

# step 3: install some necessary packages
RUN apt-get install -y -q -q --no-install-recommends g++ emacs24 vim astyle tofrodos source-highlight gdb git sqlite3 && \
    apt-get clean -y

# step 4: set up a few sanity aliases in .bashrc
RUN echo "alias mv='mv -i'" >> /root/.bashrc && \
    echo "alias rm='rm -i'" >> /root/.bashrc && \
    echo "alias cp='cp -i'" >> /root/.bashrc

# step 5: set up Ruby, as per the directions at
# https://www.brightbox.com/blog/2017/01/13/ruby-2-4-ubuntu-packages/;
# as of August 2017, that is Ruby version 2.4.1
RUN apt-add-repository -y ppa:brightbox/ruby-ng && \
    apt-get update -y -q -q && \
    apt-get install -y -q -q --no-install-recommends ruby2.4 ruby2.4-dev && \
    ruby2.4 -v && \
    update-alternatives --set ruby /usr/bin/ruby2.4 && \
    update-alternatives --set gem /usr/bin/gem2.4 && \
    apt-get clean -y && \
    ruby -v

# step 6: install rails and other gems; as of August 2017, that is
# Rails version 5.1.3 (which is explicitly installed)
RUN apt-get install -y -q -q --no-install-recommends zlib1g-dev pkg-config libsqlite3-dev && \
    echo gem: --no-ri --no-rdoc > /root/.gemrc && \
    gem install bundler && \
    gem install nokogiri && \
    gem install rails -v 5.1.3 && \
    apt-get clean -y

# step 7: create a rails app so that it downloads all the appropriate
# gems
RUN cd /root; rails new myapp && \
    sed -i "s/gem 'tzinfo-data'/#gem 'tzinfo-data'/g" /root/myapp/Gemfile && \
    echo "gem 'tzinfo-data'" >> /root/myapp/Gemfile && \
    /bin/rm -f /root/myapp/Gemfile.lock && \
    cd /root/myapp; bundle install && \
    /bin/rm -rf /root/myapp

# step 8: install nodejs, which is needed by the `rails server` comamnd
RUN apt-get install -y -q -q --no-install-recommends curl && \
    curl -sL https://deb.nodesource.com/setup_8.x | bash - && \
    apt-get install -y -q -q --no-install-recommends nodejs && \
    apt-get clean -y

# step 9: set up Python and Django; as of September 2017, this is Python
# version 3.5.2 and Django version 1.11.5 (which is explicitly installed)
RUN apt-get install -y -q -q --no-install-recommends python3-pip && \
    pip3 install --upgrade pip && \
    pip install Django==1.11.5 && \
    pip install virtualenv && \
    apt-get clean -y

# step 10: create work directory and expose the ports used with the
# development servers
RUN mkdir /root/work
EXPOSE 3000 8000

# step 11: remove the (possible) apt-cacher-ng repository
RUN /bin/rm -f /etc/apt/apt.conf.d/02proxy

# all done!
