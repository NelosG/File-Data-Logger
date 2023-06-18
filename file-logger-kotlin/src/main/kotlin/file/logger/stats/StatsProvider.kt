package trash.remote.file.stats

import file.logger.logger.Logger
import kotlinx.coroutines.*
import java.io.File
import java.nio.file.Files
import java.nio.file.attribute.BasicFileAttributes
import java.time.LocalDateTime
import java.time.ZoneId
import java.util.concurrent.Executors

class StatsProvider(private val logger: Logger, threadCount: Int = Runtime.getRuntime().availableProcessors()) :
    AutoCloseable {
    private val context = Executors.newScheduledThreadPool(threadCount).asCoroutineDispatcher()
    private var scope = CoroutineScope(context)
    private val def: MutableList<Deferred<Unit>> = mutableListOf()
    private var sheetIndx = 0

    init {
        logger.setSheet(
            "Sheet${sheetIndx++}",
            "Path", "Name", "Created", "Modified", "Acessed", "Size"
        )
    }

    fun processDir(start: File): StatsProvider {
        def.add(scope.async { processDirImpl(start) })
        return this
    }

    fun await(): StatsProvider {
        run {
            runBlocking {
                def.forEach {
                    it.await()
                }
            }
        }
        flushLogger()
        return this
    }

    fun reset(): StatsProvider {
        flushLogger()
        doStop()
        logger.setSheet(
            "Sheet${sheetIndx++}",
            "Path", "Name", "Created", "Modified", "Acessed", "Size"
        )
        return this
    }

    private fun doStop() {
        scope.cancel()
        scope = CoroutineScope(context)
        def.clear()
        flushLogger()
    }

    private fun flushLogger() {
        logger.autoSizeColumns(0)
        logger.flush()
    }

    override fun close() {
        doStop()
        context.close()
        logger.close()
    }

    private suspend fun processDirImpl(start: File) {

        val allFiles = start.listFiles() ?: return
        coroutineScope {
            allFiles.filter { it.isDirectory }.forEach {
                launch {
                    processDirImpl(it)
                }
            }
        }

        allFiles.filter { it.isFile }.forEach { file ->
            val list = arrayListOf<Any>()
            try {
                list.add(file.absolutePath)
                list.add(file.name)
                val attr = Files.readAttributes(file.toPath(), BasicFileAttributes::class.java)
                list.add(
                    LocalDateTime.ofInstant(
                        attr.creationTime().toInstant(),
                        ZoneId.systemDefault()
                    )
                )
                list.add(
                    LocalDateTime.ofInstant(
                        attr.lastModifiedTime().toInstant(),
                        ZoneId.systemDefault()
                    )
                )
                list.add(
                    LocalDateTime.ofInstant(
                        attr.lastAccessTime().toInstant(),
                        ZoneId.systemDefault()
                    )
                )
                list.add(attr.size())

                logger.println(*list.toTypedArray())
            } catch (e: Throwable) {
                println("ERR: ${e.message}")
                throw e
            }
        }
    }
}