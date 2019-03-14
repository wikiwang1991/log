#include <log.h>
#include <libwebsockets.h>
#include <thread>
#include <mutex>
#include <list>
#include "log_generated.h"

extern "C" {
int log_initialize_impl(const char *uri, log_func *func);
int log_close_impl();
}

static constexpr size_t BUFFER_SIZE = 4096;

static lws_context *context;

static std::thread thread;
static std::mutex mutex;

static std::list<log::BufferBuilder> send;

static void log_impl(LOG_ARGS) {
    char b[BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(b, fmt, ap);
    va_end(ap);

    flatbuffers::FlatBufferBuilder fbb;
    auto func = fbb.CreateString(function);
    auto msg = fbb.CreateString(b);
    auto log = log::CreateLog(fbb, 0, level, 0, 0, func, line, 0, msg);
    auto buffer = log::CreateBuffer(fbb, log::Msg_Log, log.Union());
    fbb.Finish(buffer);

    fbb.GetBufferPointer();
    fbb.GetSize();
//    lws_callback_on_writable((*ppss)->wsi);
}

static int log_server(lws *wsi, lws_callback_reasons reason,
                      void *user, void *in, size_t len) {
    switch (reason) {
    case LWS_CALLBACK_SERVER_WRITEABLE:
        break;
    case LWS_CALLBACK_RECEIVE:
    {
        auto buffer = log::GetBuffer(in);
        auto type = buffer->msg_type();
        switch (type) {
        case log::Msg_LogConfig:
        {
            auto config = static_cast<const log::LogConfig *>(buffer->msg());
            config->level();
            config->filter();
            config->object();
            config->function()->str();
            config->line();
            config->thread();
            config->regexp()->str();
            break;
        }
        }
        break;
    }
    }
    return 0;
}

int log_initialize_impl(const char *uri, log_func *func) {
    lws_protocols protocols[] = {
        { "log", log_server, 0, 0 },
        { nullptr, nullptr, 0, 0 }
    };

    lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context) return -1;

    *func = log_impl;

    thread = std::thread([]{ while (lws_service(context, -1) >= 0) {} });

    return 0;
}

int log_close_impl() {
    lws_cancel_service(context);
    thread.join();
    lws_context_destroy(context);
    return 0;
}

